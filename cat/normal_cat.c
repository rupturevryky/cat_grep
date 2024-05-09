void end_and_start_string(MyObject *obj, const char last_char, int n_place, int *checher_start,
                          int *need_dollar, int *need_r) {
    // printf("\n\nprev_char: '%c'\n\n", last_char);

    if (*need_dollar) {
        if (obj->e == 1 && last_char != '\t') printf("^M");
        if (obj->e == 1) printf("$");
        if (*need_r && obj->e != 1) printf("\r");
        *need_r = 0;
        printf("\n");
        *need_dollar = 0;
    } else {
        if (n_place == 1) {
            *checher_start = 1;
            *need_dollar = 1;
        } else {
            *checher_start = 0;
            *need_dollar = 0;
        }
    }
}

void print_line(MyObject *obj, const char *input, int n_place, int *checher_start, int *need_dollar,
                char *prev_char, int *need_r) {
    int i = 0;
    *checher_start = 0;

    end_and_start_string(obj, *prev_char, n_place, checher_start, need_dollar, need_r);

    while (input[i] != '\0' && input[i] != '\r') {
        if (obj->t == 1 && input[i] == '\t') {
            printf("^I");
        } else
            printf("%c", input[i]);

        i++;
    }
    *prev_char = input[i - 1];
    if (input[i] != '\0') *need_r = 1;
    end_and_start_string(obj, *prev_char, n_place, checher_start, need_dollar, need_r);
}
void printer(MyObject *obj, int *line, const int sizeof_string, FILE *now_file, int *checher_start,
             int *need_dollar, char *prev_char, int *squeeze_blank, int *need_r) {
    char input[sizeof_string];
    int start_file = 1;
    while (fgets(input, sizeof_string, now_file) != NULL) {
        if (input[0] == '\n' || input[0] == '\0')
            *squeeze_blank += 1;
        else
            *squeeze_blank = 0;
        if (obj->s == 1 && *squeeze_blank > 1) continue;
        if (*checher_start) *line += 1;

        int n_place = 0;
        if (strchr(input, '\n')) n_place = 1;

        input[strcspn(input, "\n")] = '\0';  // Удаляем символ новой строки

        if (((obj->b == 1 && input[0] != '\0' && input[0] != '\n') || obj->n == 1) &&
            (!start_file || *checher_start)) {
            if (*need_r) printf("\r");
            *need_r = 0;

            printf("%6d\t", *line);
        }

        print_line(obj, input, n_place, checher_start, need_dollar, prev_char, need_r);
    }
}
