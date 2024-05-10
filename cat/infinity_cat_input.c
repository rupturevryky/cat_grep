void flag_b(const int empty_cheker, int *line) {
  if (!empty_cheker) {
    printf("%6d\t", *line);
    *line += 1;
  }
}
int is_string_empty(int i, char *buffer) {
  if (buffer[i] == '\n') return 1;
  return 0;
}

void print_char_inf(char *input, int t) {
  if (strcmp(input, "\n") != 0) {
    for (int i = 0; input[i] != '\n'; i++) {
      // флаг -t
      if (t == 1 && input[i] == '\t')
        printf("^I");
      else
        printf("%c", input[i]);
    }
  }
}

void infinity_input(MyObject *obj) {
  char input[4096];
  int line = 1;
  int flag_s = 0;
  while (fgets(input, sizeof(input), stdin) != NULL) {
    int empty_string_checker = is_string_empty(0, input);

    if (obj->s == 1 && empty_string_checker && flag_s > 0)
      continue;
    else
      flag_s = 0;

    if (obj->n == 1) {
      printf("%6d\t", line);
      line++;
    };
    if (obj->b == 1) flag_b(empty_string_checker, &line);

    print_char_inf(input, obj->t);

    if (obj->e == 1) printf("$");
    printf("\n");

    if (obj->s == 1 && empty_string_checker) flag_s++;
  }
}