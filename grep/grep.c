#include <ctype.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
} MyObject;

#include "grep_flags.c"
#include "grep_printer.c"

int check_opt(MyObject *obj, int argc, char *argv[], char **pattern);

int main(int argc, char *argv[]) {
  MyObject obj = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  char *pattern = NULL;

  if (check_opt(&obj, argc, argv, &pattern) != 0) {
    return 1;
  };
  // Проверяем, что передан патерн
  if (optind == argc && obj.f == 0) {
    printf("Usage: grep [OPTION]... PATTERNS [FILE]...");
    return 1;
  }

  if (obj.f == 0 && obj.e == 0) pattern = argv[optind];

  regex_t regex;
  int compare_flgs = 1;
  if (obj.i == 1)
    compare_flgs = regcomp(&regex, pattern, REG_ICASE);
  else
    compare_flgs = regcomp(&regex, pattern, REG_EXTENDED);

  if (compare_flgs == 1) {
    if (obj.f == 1) free(pattern);
    return 1;
  }
  if (obj.f == 1) free(pattern);

  int is_other_files = 0;

  if ((optind + 1 == argc && obj.f == 0) || (optind == argc && obj.f == 1)) {
    FILE *now_file = NULL;
    char *filename = NULL;
    const int sizeof_string = 16384;
    grep_printer(&regex, &obj, sizeof_string, now_file, filename,
                 is_other_files);
    regfree(&regex);
    return 0;
  }
  optind++;
  if (optind + 1 < argc) is_other_files = 1;

  FILE *now_file = NULL;
  for (int i = optind; i < argc; i++) {
    char *filename = argv[i];
    now_file = fopen(filename, "r");

    if (!now_file) {
      if (!obj.s) printf("grep: %s: No such file or directory", filename);
      continue;
    };
    // Определение размера файла
    fseek(now_file, 0, SEEK_END);
    long fileSize = ftell(now_file);
    fileSize++;
    fseek(now_file, 0, SEEK_SET);

    grep_printer(&regex, &obj, fileSize, now_file, filename, is_other_files);

    if (now_file) fclose(now_file);
  }
  regfree(&regex);

  return 0;
}

int check_opt(MyObject *obj, int argc, char *argv[], char **pattern) {
  int option;
  while ((option = getopt(argc, argv, "e:ivclnhsof:")) != -1) {
    switch (option) {
      case 'e':
        obj->e = 1;
        *pattern = optarg;
        break;
      case 'i':
        obj->i = 1;
        break;
      case 'v':
        obj->v = 1;
        break;
      case 'c':
        obj->c = 1;
        break;
      case 'l':
        obj->l = 1;
        obj->h = 0;
        break;
      case 'n':
        obj->n = 1;
        break;
      case 'h':
        if (obj->l == 0) obj->h = 1;
        break;
      case 's':
        obj->s = 1;
        break;
      case 'f':
        if (flag_f(obj, pattern) == 1) return 1;
        break;
      case 'o':
        obj->o = 1;
        break;
      case '?':
        exit(1);
      default:
        abort();
    }
  }
  return 0;
}
