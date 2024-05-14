void end_and_start_string(MyObject *obj, int n_placed, int *checher_start,
                          int *need_dollar, int *need_r) {
  if (*need_dollar) {
    if (obj->v == 1 && *need_r) printf("^M");
    if (obj->e == 1) printf("$");
    if (*need_r && obj->v == 0) printf("\r");
    *need_r = 0;
    printf("\n");
    *need_dollar = 0;
  } else {
    if (n_placed == 1) {
      *checher_start = 1;
      *need_dollar = 1;
    } else {
      *checher_start = 0;
      *need_dollar = 0;
    }
  }
}

void print_line(MyObject *obj, const char *input, int n_placed,
                int *checher_start, int *need_dollar, char *prev_char,
                int *need_r) {
  int i = 0;
  *checher_start = 0;

  end_and_start_string(obj, n_placed, checher_start, need_dollar, need_r);

  while (input[i] != '\0' && input[i] != '\r') {
    if (obj->t == 1 && input[i] == '\t') {
      printf("^I");
    } else if (obj->v == 0)
      printf("%c", input[i]);
    else
      flag_v(input[i]);

    i++;
  }
  if (i > 0)
    *prev_char = input[i - 1];
  else
    *prev_char = input[i];

  if (input[i] != '\0') *need_r = 1;
  end_and_start_string(obj, n_placed, checher_start, need_dollar, need_r);
}
void printer(MyObject *obj, int *line, const int sizeof_string, FILE *now_file,
             int *checher_start, int *need_dollar, char *prev_char,
             int *squeeze_blank, int *need_r) {
  char input[sizeof_string];
  int start_file = 1;
  while (fgets(input, sizeof_string, now_file) != NULL) {
    if (input[0] == '\n' || input[0] == '\0')
      *squeeze_blank += 1;
    else
      *squeeze_blank = 0;
    if (obj->s == 1 && *squeeze_blank > 1) continue;
    if (*checher_start && ((obj->b == 1 && *squeeze_blank == 0) || obj->n == 1))
      *line += 1;

    int n_placed = 0;
    if (strchr(input, '\n')) n_placed = 1;

    input[strcspn(input, "\n")] = '\0';  // Удаляем символ новой строки

    if (((obj->b == 1 && input[0] != '\0' && input[0] != '\n') ||
         obj->n == 1) &&
        (!start_file || *checher_start)) {
      if (*need_r) {
        printf("\r");
        *need_r = 0;
      }
      printf("%6d\t", *line);
    }

    print_line(obj, input, n_placed, checher_start, need_dollar, prev_char,
               need_r);
  }
}
