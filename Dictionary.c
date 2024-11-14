#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char *filename = "dictionary.txt";
  FILE *f;
  char c;
  if((f=fopen(filename, "a+")) == NULL) {
    puts("File cannot be opened!");
    exit(1);
  }

    do {
      printf("* OPTIONS *\n");
      printf("1. Bahasa Inggris - Batak\n");
      printf("2. Batak - Bahasa Inggris\n");
      printf("3. Add New Word : \n");
      printf("Enter your option : ");

      int opt;
      scanf("%d", &opt);

      char word[255];
      char hata[255];

      switch (opt)
      {
        case 1:
         printf("Word: ");
         scanf("%s", word);
        
        char buff[1024];

        while(fgets(buff, 1024, f)) {

          buff[strcspn(buff, "\n")] = 0;

          char *token = strtok(buff, "|");
          char *wordFromFile, *hataFromFile;

          int i=0;
          while(token != NULL) {
            if(i == 0) {
              hataFromFile = token;
            } else {
              wordFromFile = token;
            }

            token = strtok(NULL, "|");
            i++;
          }

          if(strcmp(word, wordFromFile) == 0) {
            printf("Means : %s\n", hataFromFile);
            break;
          }
        }
        
        break;

        case 2:
         printf("Word : ");
         scanf("%s", hata);
        
        char buff2[1024];

        while(fgets(buff2, 1024, f)) {

          buff[strcspn(buff2, "\n")] = 0;

          char *token = strtok(buff2, "|");
          char *wordFromFile, *hataFromFile;

          int i=0;
          while(token != NULL) {
            if(i == 0) {
              hataFromFile = token;
            } else {
              wordFromFile = token;
            }

            token = strtok(NULL, "|");
            i++;
          }

          if(strcmp(hata, hataFromFile) == 0) {
            printf("Means : %s\n", wordFromFile);
            break;
          }
        }
        
        break;

        case 3:
         printf("Add New Word : ");
          c = getchar();
          if((f=fopen(filename, "a+")) == NULL) {
            puts("File cannot be opened!");
            exit(1);
          }
         while((c = getchar()) != '\n' && c != EOF) {
          putc(c, f);
         }
         
         putc('\n', f);
         fclose(f);
         break;

        default:
         printf("Invalid options");
         break;
      }
      printf("\n");
    } while(0);
    system("PAUSE");

    return 0;
}