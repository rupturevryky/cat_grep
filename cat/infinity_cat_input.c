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

void flag_v(char ch) {
  if (0 < ch && ch <= 31 && ch != '\t' && ch != '\n')
    printf("^%c", ch + 64);
  else if (ch == 127) {
    printf("^?");
  } else
    printf("%c", ch);
}

void print_char_inf(char *input, MyObject *obj) {
  if (strcmp(input, "\n") != 0) {
    for (int i = 0; input[i] != '\n' && input[i] != '\0'; i++) {
      // флаг -t
      if (obj->t == 1 && input[i] == '\t')
        printf("^I");
      else if (obj->v == 0)
        printf("%c", input[i]);
      else
        flag_v(input[i]);
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

    print_char_inf(input, obj);

    if (obj->e == 1) printf("$");
    printf("\n");

    if (obj->s == 1 && empty_string_checker) flag_s++;
  }
}