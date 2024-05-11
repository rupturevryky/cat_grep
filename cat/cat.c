#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int b;
  int e;
  int n;
  int s;
  int t;
} MyObject;

#include "infinity_cat_input.c"
#include "normal_cat.c"

void check_opt(MyObject *obj, int argc, char *argv[]);

int main(int argc, char *argv[]) {
  MyObject obj = {0, 0, 0, 0, 0};

  // Проверяем, что переданы аргументы командной строки
  if (argc < 2) infinity_input(&obj);

  // считываем флаги и параметры
  check_opt(&obj, argc, argv);
  if (optind >= argc) infinity_input(&obj);

  int line = 0;
  int checher_start = 1;
  int need_dollar = 0;
  int need_r = 0;
  char prev_char = '\n';
  int squeeze_blank = 0;

  FILE *now_file = NULL;
  // бежит по файлам
  for (int i = optind; i < argc; i++) {
    // текущий файл
    char *filename = argv[i];
    now_file = fopen(filename, "r");
    // Проверяем, удалось ли открыть файл
    if (!now_file) {
      printf("cat: %s: No such file or directory\n", filename);
      continue;
    }
    // Определение размера файла
    fseek(now_file, 0, SEEK_END);
    long fileSize = ftell(now_file);
    fileSize++;
    fseek(now_file, 0, SEEK_SET);

    printer(&obj, &line, fileSize, now_file, &checher_start, &need_dollar,
            &prev_char, &squeeze_blank, &need_r);

    if (now_file) fclose(now_file);
  }
  return 0;
}

void check_opt(MyObject *obj, int argc, char *argv[]) {
  static struct option options[] = {
      {"number-nonblank", no_argument, 0, 'b'},
      {"number", no_argument, 0, 'n'},
      {"squeeze-blank", no_argument, 0, 's'},
      {0, 0, 0, 0},
  };

  int option;
  while ((option = getopt_long(argc, argv, "benstET", options, NULL)) != EOF) {
    switch (option) {
      case 'b':
        obj->b = 1;
        obj->n = 0;
        break;
      case 'n':
        if (!obj->b) obj->n = 1;
        break;
      case 'e':
        obj->e = 1;
        break;
      case 'E':
        obj->e = 1;
        break;
      case 's':
        obj->s = 1;
        break;
      case 't':
        obj->t = 1;
        break;
      case 'T':
        obj->t = 1;
        break;
      case '?':
        exit(1);
      default:
        abort();
    }
  }
}