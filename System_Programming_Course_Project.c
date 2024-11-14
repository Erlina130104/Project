#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <pthread.h>

#define SHM_KEY 1234
#define MSG_KEY 5678

#define PEMAIN1 1
#define PEMAIN2 2

#define PILIHAN_TIDAK_ADA 0
#define PILIHAN_BATU 1
#define PILIHAN_KERTAS 2
#define PILIHAN_GUNTING 3

// Struktur untuk menyimpan data bersama antara pemain
struct SharedMemory {
    int pilihan_pemain1;
    int pilihan_pemain2;
};

// Struktur untuk pesan yang dikirim antar pemain
struct Message {
    long mtype;
    int pilihan_pemain;
};

pthread_mutex_t mutex; // Mutex untuk sinkronisasi akses ke shared memory
int shmid, msqid;      // ID untuk shared memory dan message queue

int skor_pemain1 = 0;
int skor_pemain2 = 0;

int pilihan_pemain1_count = 0;
int pilihan_pemain2_count = 0;

// Fungsi yang akan dijalankan oleh thread pemain
void *fungsi_pemain(void *id_pemain) {
    int pemain = *(int *)id_pemain;
    struct SharedMemory *shared_memory = shmat(shmid, NULL, 0);

    struct Message msg;
    msg.mtype = pemain;

    while (1) {
        // Mengunci mutex untuk mengakses shared memory
        pthread_mutex_lock(&mutex);
        int *pilihan_pemain = (pemain == PEMAIN1) ? &shared_memory->pilihan_pemain1 : &shared_memory->pilihan_pemain2;
        int *pilihan_pemain_count = (pemain == PEMAIN1) ? &pilihan_pemain1_count : &pilihan_pemain2_count;
        // Melepas kunci mutex setelah selesai mengakses shared memory
        pthread_mutex_unlock(&mutex);

        if (*pilihan_pemain_count < 3) {
            // Meminta input dari pemain
            printf("Pemain %d, masukkan pilihan (1: Batu, 2: Kertas, 3: Gunting, 0: Keluar): ", pemain);
            fflush(stdout);
            scanf("%d", pilihan_pemain);

            // Mengunci mutex sebelum mengakses shared memory untuk mengirim pesan ke pemain lain
            pthread_mutex_lock(&mutex);
            msg.pilihan_pemain = *pilihan_pemain;
            msgsnd(msqid, &msg, sizeof(struct Message), 0);
            // Melepas kunci mutex setelah selesai mengakses shared memory
            pthread_mutex_unlock(&mutex);

            if (*pilihan_pemain == 0) {
                break;
            }

            // Tunggu pemain lain membuat pilihan
            sleep(2);

            // Mengunci mutex sebelum mengakses shared memory untuk mengupdate jumlah pilihan pemain
            pthread_mutex_lock(&mutex);
            (*pilihan_pemain_count)++;
            // Melepas kunci mutex setelah selesai mengakses shared memory
            pthread_mutex_unlock(&mutex);
        } else {
            break;
        }
    }

    // Melepaskan shared memory setelah selesai
    shmdt(shared_memory);
    pthread_exit(NULL);
}

// Fungsi yang akan dijalankan oleh thread wasit
void *fungsi_wasit(void *arg) {
    struct SharedMemory *shared_memory = shmat(shmid, NULL, 0);

    while (1) {
        // Mengunci mutex untuk mengakses shared memory
        pthread_mutex_lock(&mutex);
        int pilihan_pemain1 = shared_memory->pilihan_pemain1;
        int pilihan_pemain2 = shared_memory->pilihan_pemain2;
        // Melepas kunci mutex setelah selesai mengakses shared memory
        pthread_mutex_unlock(&mutex);

        if (pilihan_pemain1 != PILIHAN_TIDAK_ADA && pilihan_pemain2 != PILIHAN_TIDAK_ADA) {
            // Menampilkan pilihan pemain
            printf("Pemain 1 memilih: %d\n", pilihan_pemain1);
            printf("Pemain 2 memilih: %d\n", pilihan_pemain2);

            // Mengunci mutex sebelum mengakses shared memory untuk mengupdate skor dan hasil
            pthread_mutex_lock(&mutex);

            if (pilihan_pemain1 == pilihan_pemain2) {
                printf("Hasil: Seri\n");
            } else if (
                (pilihan_pemain1 == PILIHAN_BATU && pilihan_pemain2 == PILIHAN_GUNTING) ||
                (pilihan_pemain1 == PILIHAN_KERTAS && pilihan_pemain2 == PILIHAN_BATU) ||
                (pilihan_pemain1 == PILIHAN_GUNTING && pilihan_pemain2 == PILIHAN_KERTAS)) {
                printf("Hasil: Pemain 1 menang!\n");
                skor_pemain1++;
            } else {
                printf("Hasil: Pemain 2 menang!\n");
                skor_pemain2++;
            }

            // Melepas kunci mutex setelah selesai mengakses shared memory
            pthread_mutex_unlock(&mutex);

            // Menunggu pemain membuat pilihan
            sleep(2);

            // Mengunci mutex sebelum mengakses shared memory untuk mereset pilihan pemain
            pthread_mutex_lock(&mutex);
            shared_memory->pilihan_pemain1 = PILIHAN_TIDAK_ADA;
            shared_memory->pilihan_pemain2 = PILIHAN_TIDAK_ADA;
            pilihan_pemain1_count = 0;
            pilihan_pemain2_count = 0;
            // Melepas kunci mutex setelah selesai mengakses shared memory
            pthread_mutex_unlock(&mutex);

            // Menampilkan skor
            printf("Skor: Pemain 1 - %d, Pemain 2 - %d\n", skor_pemain1, skor_pemain2);
        }
    }

    // Melepaskan shared memory setelah selesai
    shmdt(shared_memory);
    pthread_exit(NULL);
}

int main() {
    // Inisialisasi shared memory
    shmid = shmget(SHM_KEY, sizeof(struct SharedMemory), IPC_CREAT | 0666);

    // Inisialisasi message queue
    msqid = msgget(MSG_KEY, IPC_CREAT | 0666);

    // Inisialisasi mutex
    pthread_mutex_init(&mutex, NULL);

    // Inisialisasi thread
    pthread_t thread_pemain1, thread_pemain2, thread_wasit;

    // Membuat thread pemain 1
    int id_pemain1 = PEMAIN1;
    pthread_create(&thread_pemain1, NULL, fungsi_pemain, (void *)&id_pemain1);

    // Membuat thread pemain 2
    int id_pemain2 = PEMAIN2;
    pthread_create(&thread_pemain2, NULL, fungsi_pemain, (void *)&id_pemain2);

    // Membuat thread wasit
    pthread_create(&thread_wasit, NULL, fungsi_wasit, NULL);

    // Menunggu thread pemain 1 selesai
    pthread_join(thread_pemain1, NULL);

    // Menunggu thread pemain 2 selesai
    pthread_join(thread_pemain2, NULL);

    // Menunggu thread wasit selesai
    pthread_join(thread_wasit, NULL);

    // Menghapus shared memory
    shmctl(shmid, IPC_RMID, NULL);

    // Menghapus message queue
    msgctl(msqid, IPC_RMID, NULL);

    // Menghancurkan mutex
    pthread_mutex_destroy(&mutex);

    return 0;
}
