#ifndef GREPFLAGS
#define GREPFLAGS

int flag_f(MyObject *obj, char **pattern) {
  obj->f = 1;

  FILE *reg_ex_file = fopen(optarg, "r");

  if (reg_ex_file != NULL) {
    size_t len = 0;
    // Читаем содержимое файла в буфер
    getline(pattern, &len, reg_ex_file);
    if ((*pattern)[strlen(*pattern) - 1] == '\n')
      (*pattern)[strlen(*pattern) - 1] = '\0';

    // Закрываем файл
    fclose(reg_ex_file);
    if (pattern == NULL) return 1;

    return 0;
  }
  printf("grep: %s: No such file or directory", optarg);
  return 1;
};

void flag_n(
    int line, MyObject *obj, char prev_char, int i,
    int should) {  // should - принуждение для срабатывания, например с флагом o
  if (obj->n == 1 && (should || i == 0 || prev_char == '\n')) {
    if (isatty(fileno(stdout)))
      printf("\033[0;32m%d\033[0m\033[0;36m:\033[0m", line);
    else
      printf("%d:", line);
  }
}
void print_now_file(const char *file_name, char prev_char, int i, MyObject *obj,
                    int is_other_files, int should_word) {
  if (obj->h == 0 && is_other_files && file_name &&
      (prev_char == '\n' || i == 0 || should_word)) {
    if (isatty(fileno(stdout)))
      printf("\033[0;35m%s\033[0m\033[0;36m:\033[0m", file_name);
    else
      printf("%s:", file_name);
  }
}

void flag_o(int line, MyObject *obj, char prev_char, int i, int updateble,
            const char *file_name, int is_other_files) {
  if (obj->o == 1 && updateble) {
    printf("\n");
    print_now_file(file_name, prev_char, i, obj, is_other_files, 1);
    flag_n(line, obj, prev_char, i, 1);
  }
}

int flag_c(MyObject *obj, int *line_c, int end, const char *file_name,
           int is_other_files) {
  if (obj->c == 1) {
    if (end) {  // скипать больше некуда
      print_now_file(file_name, '\n', 0, obj, is_other_files, 0);
      printf("%d\n", *line_c);
      return 0;
    }
    // скипаем дальше считаем линии
    *line_c += 1;
    return 0;
  }
  return 1;  // obj->c == 0 - не скипаем, печатаем основной функционал grep
}

int flag_l(MyObject *obj, const char *file_name) {
  if (obj->l == 1) {
    if (!file_name) {
      if (isatty(fileno(stdout)))
        printf("\033[38;5;129m(standard input)\033[0m");
      else
        printf("(standard input)");
    } else {
      if (isatty(fileno(stdout)))
        printf("\033[0;35m%s\033[0m", file_name);
      else
        printf("%s", file_name);
      printf("\n");
    }
    return 0;
  }
  return 1;
}

void flag_v(MyObject *obj, int *reti) {
  if (obj->v == 1) *reti = !*reti;
}

#endif