#include <dirent.h>
#include <stdio.h>
#include <string.h>

int func_print(const char* name);
int func_add(const char* nameadd);
int shifr();

int main() {
  printf("Выберите режим:\n");
  printf(" 1 - вывод текстового файла\n");
  printf(" 2 - дополнение файла в конце\n");
  printf(" 3 - шифрование файлов\n");
  printf(" -1 - выход\n");
  int choice;
  char file_name[255];
  int set = 0;
  do {
    scanf("%d", &choice);

    switch (choice) {
      case 1:
        printf("Ведите имя файла:\n");
        scanf("%s", file_name);
        set = 1;
        func_print(file_name);
        break;

      case 2:
        printf("Дополните файл любой надписью:\n");
        if (set != 1) {
          printf("n/a\n");
          return 1;
        }
        func_add(file_name);
        func_print(file_name);
        break;
      case 3:
        shifr();
        break;
      case -1:
        break;

      default:
        printf("n/a\n");
    }
  } while (choice != -1);

  return 0;
}

int shifr() {
  char directory[] = "./ai_modules";
  struct dirent* entry;
  DIR* dp = opendir(directory);
  int key;
    printf("Введите сдвиг:\n");
  scanf("%d", &key);

  if (dp == NULL) {
    printf("n/a\n");
    return 1;
  }

  while ((entry = readdir(dp))) {
    if (strstr(entry->d_name, ".c") != NULL) {
      char file_path[512];
      snprintf(file_path, sizeof(file_path), "%s/%s", directory, entry->d_name);

      FILE* file = fopen(file_path, "r+");

      if (file) {
        char ch;

        while ((ch = fgetc(file)) != EOF) {
          if (ch >= 'a' && ch <= 'z') {
            ch = (ch - 'a' + key) % 26 + 'a';
          }

          else if (ch >= 'A' && ch <= 'Z') {
            ch = (ch - 'A' + key) % 26 + 'A';
          }
          fseek(file, -1, SEEK_CUR);
          fputc(ch, file);
        }
        fclose(file);
      }
    }

    if (strstr(entry->d_name, ".h") != NULL) {
      char file_path[512];
      snprintf(file_path, sizeof(file_path), "%s/%s", directory, entry->d_name);

      FILE* file = fopen(file_path, "w");

      if (file) {
        fclose(file);
      }
    }
  }
  closedir(dp);
  return 0;
}

int func_print(const char* name) {
  
  FILE* file = fopen(name, "rt");

  if (file == NULL) {
    printf("n/a\n");
    return 0;
  }

  char ch;
  while ((ch = getc(file)) != EOF) {
    putchar(ch);
  }

  fclose(file);
  printf("\n");
  return 1;
}

int func_add(const char* nameadd) {
  
  char str[255];
  scanf("%s", str);

  FILE* file2 = fopen(nameadd, "at");

  if (file2 == NULL) {
    printf("n/a\n");
    return 0;
  }
  fprintf(file2, "\n%s", str);
  fclose(file2);
  return 1;
}
