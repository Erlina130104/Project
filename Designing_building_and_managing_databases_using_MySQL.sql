-- Membuat database
CREATE DATABASE Proyek_pbd;
USE Proyek_pbd;

-- Tabel Pengunjung
CREATE TABLE Pengunjung (
  id INT AUTO_INCREMENT PRIMARY KEY,
  nama VARCHAR(50) NOT NULL,
  alamat VARCHAR(100) NOT NULL,
  email VARCHAR(50) NOT NULL,
  no_telepon VARCHAR(20) NOT NULL
);
-- Menyisipkan data ke dalam tabel Pengunjung
INSERT INTO Pengunjung (nama, alamat, email, no_telepon)
VALUES ('John Doe', 'Jl. Merdeka No. 123', 'johndoe@gmail.com', '08123456789'),
       ('Jane Smith', 'Jl. Sudirman No. 456', 'janesmith@gmail.com', '08987654321'),
       ('Mark Johnson', 'Jl. Ahmad Yani No. 789', 'markjohnson@gmail.com', '08567891234'),
       ('Sarah Williams', 'Jl. Gatot Subroto No. 321', 'sarahwilliams@gmail.com', '08765432109');

-- Tabel Wisata
CREATE TABLE Wisata (
  id INT AUTO_INCREMENT PRIMARY KEY,
  nama VARCHAR(100) NOT NULL,
  kategori VARCHAR(50) NOT NULL,
  alamat VARCHAR(100) NOT NULL,
  deskripsi TEXT,
  harga DECIMAL(10, 2) NOT NULL,
  jam_buka TIME NOT NULL,
  gambar VARCHAR(100)
);
-- Menyisipkan data ke dalam tabel Wisata
INSERT INTO Wisata (nama, kategori, alamat, deskripsi, harga, jam_buka, gambar)
VALUES ('Pantai Indah', 'Pantai', 'Jl. Pantai Indah No. 1', 'Pantai yang indah dengan pasir putih dan air jernih.', 50.00, '09:00:00', 'pantai_indah.jpg'),
       ('Gunung Permai', 'Gunung', 'Jl. Gunung Permai No. 2', 'Gunung dengan pemandangan indah dan jalur pendakian yang menantang.', 100.00, '07:00:00', 'gunung_permai.jpg'),
       ('Taman Wisata Alam', 'Taman', 'Jl. Taman Wisata No. 3', 'Taman yang luas dengan beragam flora dan fauna.', 20.00, '08:00:00', 'taman_wisata_alam.jpg');
-- Tabel Ulasan
CREATE TABLE Ulasan (
  id INT AUTO_INCREMENT PRIMARY KEY,
  pengunjung_id INT,
  wisata_id INT,
  rating INT NOT NULL,
  komentar TEXT,
  FOREIGN KEY (pengunjung_id) REFERENCES Pengunjung(id),
  FOREIGN KEY (wisata_id) REFERENCES Wisata(id)
);
-- Menyisipkan data ke dalam tabel Ulasan
INSERT INTO Ulasan (pengunjung_id, wisata_id, rating, komentar)
VALUES (1, 1, 4, 'Pantai Indah sangat menakjubkan. Pemandangannya luar biasa!'),
       (2, 2, 5, 'Gunung Permai luar biasa! Pendakiannya sulit tetapi sangat menyenangkan.'),
       (3, 3, 3, 'Taman Wisata Alam memiliki banyak pilihan aktivitas menarik.'),
       (4, 1, 2, 'Pantai Indah tidak sesuai harapan. Terlalu ramai dan kotor.');


-- Tabel Admin
CREATE TABLE ADMIN (
  id INT AUTO_INCREMENT PRIMARY KEY,
  nama VARCHAR(50) NOT NULL,
  username VARCHAR(50) NOT NULL,
  PASSWORD VARCHAR(50) NOT NULL
);
-- Menyisipkan data ke dalam tabel Admin
INSERT INTO ADMIN (nama, username, PASSWORD)
VALUES ('Admin1', 'admin1', 'admin123'),
       ('Admin2', 'admin2', 'admin456');

-- Tabel Tiket
CREATE TABLE Tiket (
  id INT AUTO_INCREMENT PRIMARY KEY,
  pengunjung_id INT,
  wisata_id INT,
  jumlah_tiket INT NOT NULL,
  tanggal DATE NOT NULL,
  FOREIGN KEY (pengunjung_id) REFERENCES Pengunjung(id),
  FOREIGN KEY (wisata_id) REFERENCES Wisata(id)
);
-- Menyisipkan data ke dalam tabel Tiket
INSERT INTO Tiket (pengunjung_id, wisata_id, jumlah_tiket, tanggal)
VALUES (1, 1, 2, '2023-05-17'),
       (2, 2, 3, '2023-05-18'),
       (3, 3, 1, '2023-05-19'),
       (4, 1, 4, '2023-05-20');
       
       
-- 10 query yang menampilkan data dari 1 table

-- Menampilkan semua data dalam tabel wisata: 
 SELECT * FROM Wisata;
 
 -- Menampilkan nama dan harga tiket dari semua tempat wisata:
 SELECT nama, harga FROM Wisata;
 
-- Menampilkan jumlah  total pengunjung yang telah mengulas tempat wisata:
SELECT COUNT(DISTINCT pengunjung_id) AS total_pengunjung
FROM Ulasan;

-- Menampilkan jumlah ulasan yang diberikan untuk setiap tempat wisata:
SELECT wisata_id, COUNT(*) AS jumlah_ulasan
FROM Ulasan
GROUP BY wisata_id;

-- Menampilkan tempat wisata dengan total harga tiket lebih dari 200:
SELECT Wisata.nama, SUM(Tiket.jumlah_tiket * Wisata.harga) AS total_harga
FROM Wisata
INNER JOIN Tiket ON Wisata.id = Tiket.wisata_id
GROUP BY Wisata.id
HAVING total_harga > 200;

-- Menampilkan jumlah tiket yang telah dipesan pada tanggal tertentu:
SELECT tanggal, SUM(jumlah_tiket) AS total_tiket
FROM Tiket
WHERE tanggal = '2023-05-17'
GROUP BY tanggal;

-- Menampilkan tempat wisata dengan jumlah ulasan lebih dari 0:
SELECT w.nama, COUNT(u.wisata_id) AS jumlah_ulasan
FROM Wisata AS w
LEFT JOIN Ulasan AS u ON w.id = u.wisata_id
GROUP BY w.id
HAVING COUNT(u.wisata_id) > 0;

-- Menampilkan tempat wisata yang memiliki deskripsi lebih dari 10 karakter:
SELECT *
FROM Wisata
WHERE LENGTH(deskripsi) > 10;
 
-- Menampilkan tempat wisata dengan harga tiket maksimum:
SELECT *
FROM Wisata
WHERE harga = (SELECT MAX(harga) FROM Wisata);

-- Menampilkan tempat wisata yang dibuka setelah pukul 8 pagi:
SELECT * FROM Wisata WHERE jam_buka > '08:00:00';

-- 10 query yang menampilkan data dari beberapa tabel dengan menggunakan inner JOIN, left JOIN, dan right JOIN:

-- Nomor 1:
-- Menampilkan nama pengunjung dan nama tempat wisata yang telah dikunjungi oleh pengunjung:
SELECT Pengunjung.nama, Wisata.nama AS nama_tempat_wisata
FROM Pengunjung
INNER JOIN Ulasan ON Pengunjung.id = Ulasan.pengunjung_id
INNER JOIN Wisata ON Ulasan.wisata_id = Wisata.id;

-- Nomor 2:
-- Menampilkan nama pengunjung dan total jumlah tiket yang telah dibeli oleh masing-masing pengunjung:
SELECT Pengunjung.nama, SUM(Tiket.jumlah_tiket) AS total_tiket
FROM Pengunjung
LEFT JOIN Tiket ON Pengunjung.id = Tiket.pengunjung_id
GROUP BY Pengunjung.id;

-- Nomor 3:
-- Menampilkan nama pengunjung dan nama tempat wisata yang telah dikunjungi oleh pengunjung, termasuk pengunjung yang belum mengulas:
SELECT Pengunjung.nama, Wisata.nama AS nama_tempat_wisata
FROM Pengunjung
LEFT JOIN Ulasan ON Pengunjung.id = Ulasan.pengunjung_id
LEFT JOIN Wisata ON Ulasan.wisata_id = Wisata.id;

-- Nomor 4:
-- Menampilkan nama tempat wisata dan jumlah ulasan yang diberikan untuk setiap tempat wisata:
SELECT Wisata.nama, COUNT(Ulasan.id) AS jumlah_ulasan
FROM Wisata
LEFT JOIN Ulasan ON Wisata.id = Ulasan.wisata_id
GROUP BY Wisata.id;

-- Nomor 5:
-- Menampilkan nama tempat wisata, jumlah tiket yang telah dipesan, dan total harga tiket untuk setiap tempat wisata:
SELECT Wisata.nama, SUM(Tiket.jumlah_tiket) AS total_tiket, SUM(Tiket.jumlah_tiket * Wisata.harga) AS total_harga
FROM Wisata
LEFT JOIN Tiket ON Wisata.id = Tiket.wisata_id
GROUP BY Wisata.id;

-- Nomor 6:
-- Menampilkan nama pengunjung, nama tempat wisata, dan tanggal kunjungan pengunjung:
SELECT Pengunjung.nama, Wisata.nama AS nama_tempat_wisata, Tiket.tanggal
FROM Pengunjung
INNER JOIN Tiket ON Pengunjung.id = Tiket.pengunjung_id
INNER JOIN Wisata ON Tiket.wisata_id = Wisata.id;

-- Nomor 7:
-- Menampilkan nama pengunjung dan nama tempat wisata yang telah dikunjungi oleh pengunjung, hanya jika pengunjung telah memberikan ulasan:
SELECT Pengunjung.nama, Wisata.nama AS nama_tempat_wisata
FROM Pengunjung
INNER JOIN Ulasan ON Pengunjung.id = Ulasan.pengunjung_id
INNER JOIN Wisata ON Ulasan.wisata_id = Wisata.id;

-- Nomor 8:
-- Menampilkan nama pengunjung, nama tempat wisata, dan jumlah tiket yang telah dipesan oleh pengunjung, termasuk pengunjung yang belum melakukan pemesanan:
SELECT Pengunjung.nama, Wisata.nama AS nama_tempat_wisata, COALESCE(SUM(Tiket.jumlah_tiket), 0) AS total_tiket
FROM Pengunjung
LEFT JOIN Tiket ON Pengunjung.id = Tiket.pengunjung_id
LEFT JOIN Wisata ON Tiket.wisata_id = Wisata.id
GROUP BY Pengunjung.id, Wisata.id;

-- Nomor 9:
-- Menampilkan nama pengunjung, nama tempat wisata, dan tanggal kunjungan pengunjung yang memiliki ulasan:
SELECT Pengunjung.nama, Wisata.nama AS nama_tempat_wisata, Tiket.tanggal
FROM Pengunjung
INNER JOIN Ulasan ON Pengunjung.id = Ulasan.pengunjung_id
INNER JOIN Wisata ON Ulasan.wisata_id = Wisata.id
INNER JOIN Tiket ON Pengunjung.id = Tiket.pengunjung_id AND Wisata.id = Tiket.wisata_id;

-- Nomor 10
-- Menampilkan nama tempat wisata dan jumlah ulasan yang diterima oleh tempat wisata, hanya jika jumlah ulasan lebih dari 1:
SELECT Wisata.nama, COUNT(Ulasan.id) AS jumlah_ulasan
FROM Wisata
LEFT JOIN Ulasan ON Wisata.id = Ulasan.wisata_id
GROUP BY Wisata.id
HAVING COUNT(Ulasan.id) > 1;

-- 5 penggunaan subquery
-- Nomor 1:
-- Menggunakan Subquery dalam SELECT Statement:
SELECT Pengunjung.nama, (SELECT COUNT(*) FROM Tiket WHERE Tiket.pengunjung_id = Pengunjung.id) AS jumlah_tiket
FROM Pengunjung;

-- Nomor 2:
-- Menggunakan Subquery dalam WHERE Clause:
SELECT nama
FROM Pengunjung
WHERE id IN (SELECT pengunjung_id FROM Tiket WHERE DATE(tanggal) = CURDATE());

-- Nomor 3:
-- Menggunakan Subquery dalam FROM Clause:
SELECT t1.nama, t2.jumlah_ulasan
FROM (SELECT Pengunjung.nama, COUNT(*) AS jumlah_ulasan FROM Pengunjung INNER JOIN Ulasan ON Pengunjung.id = Ulasan.pengunjung_id GROUP BY Pengunjung.id) AS t1
INNER JOIN (SELECT Pengunjung.nama, COUNT(*) AS jumlah_ulasan FROM Pengunjung INNER JOIN Ulasan ON Pengunjung.id = Ulasan.pengunjung_id WHERE Ulasan.rating > 3 GROUP BY Pengunjung.id) AS t2 ON t1.nama = t2.nama;

-- Nomor 4 :
-- Menggunakan Subquery dalam HAVING Clause:
SELECT id_wisata, COUNT(*) AS jumlah_pengunjung
FROM Kunjungan
GROUP BY id_wisata
HAVING COUNT(*) > 0;

CREATE TABLE Kunjungan (
    id INT AUTO_INCREMENT,
    id_wisata INT,
    pengunjung_id INT,
    tanggal DATE,
    PRIMARY KEY (id),
    FOREIGN KEY (id_wisata) REFERENCES Wisata(id),
    FOREIGN KEY (pengunjung_id) REFERENCES Pengunjung(id)
);
INSERT INTO Kunjungan (id_wisata, tanggal) VALUES
(1, '2023-01-01'),
(2, '2023-01-02'),
(1, '2023-01-03'),
(3, '2023-01-04'),
(2, '2023-01-05');

-- Nomor 5
SELECT nama
FROM Pengunjung
WHERE id IN (SELECT pengunjung_id FROM Kunjungan);

ALTER TABLE Kunjungan
ADD COLUMN pengunjung_id INT,
ADD FOREIGN KEY (pengunjung_id) REFERENCES Pengunjung(id);


