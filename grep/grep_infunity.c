void take_correct_edges(char *input, int *pattern_end, int *pattern_index) {
    if (isascii(input[*pattern_end])) return;

    float count_ru = 0;
    int count_else = 0;
    int end = *pattern_end;
    int i = *pattern_index;

    for (i = i; i <= end; i++) {
        if (!isascii(input[i]))
            count_ru += 0.5;
        else
            count_else++;
    }

    if (count_ru != (int)count_ru) {
        count_ru += 0.5;
        end++;
    }

    int start_len = *pattern_end - *pattern_index + 1;
    float need_len = count_ru * 2 + count_else;
    if (start_len == need_len) return;
    int flag_to_pull_ru = 0;
    for (i = i; i <= end; i++) {
        if (!isascii(input[i])) {
            flag_to_pull_ru++;
            if (flag_to_pull_ru >= 2) {
                end++;
                flag_to_pull_ru = 0;
            }
        }
    }
    if (*pattern_end < end) *pattern_end = end;
}

void next_found(char *input, regex_t *regex, int *pattern_index, int *reti, int *pattern_end, int *updateble,
                regmatch_t match, int i) {
    *reti = regexec(regex, input + i, 1, &match, 0);
    if (!*reti) {
        *pattern_index = match.rm_so + i;
        if (i > 1 || *pattern_index > 1)
            *pattern_end = match.rm_eo + i - 1;
        else
            *pattern_end = match.rm_eo + i;
    }

    if (*reti)
        *updateble = 0;
    else {
        if (i >= *pattern_end && !(!*reti && *pattern_index > *pattern_end && i <= *pattern_end)) {
            if (i > 1 || *pattern_index > 1)
                *pattern_end = match.rm_eo + i - 1;
            else
                *pattern_end = match.rm_eo + i;
        }
        take_correct_edges(input, pattern_end, pattern_index);
    }
}

void print_char_in_infinuty(int *pattern_index, int *pattern_end, int *updateble, char *input, int *reti,
                            regex_t *regex, int i, MyObject *obj, int line, regmatch_t match,
                            int *now_pattern_start, int *now_pattern_end, const char *file_name) {
    if (*updateble && i > *pattern_index && i > *now_pattern_end)
        next_found(input, regex, pattern_index, reti, pattern_end, updateble, match, i);

    if (i > *now_pattern_end) {
        *now_pattern_start = *pattern_index;
        *now_pattern_end = *pattern_end;
        flag_o(line, obj, input[i - 1] || '\0', i, *updateble, file_name);
    }

    if (i >= *now_pattern_start && i <= *now_pattern_end) {
        if (isatty(fileno(stdout))) {
            if (i == *now_pattern_start) printf("\033[1;31m");
            printf("%c", input[i]);
            if (i == *now_pattern_end) printf("\033[0m");
        } else
            printf("%c", input[i]);
    } else if (obj->o == 0)
        printf("%c", input[i]);
};

void check_line_for_pattern(char *input, regex_t *regex, int line, MyObject *obj, int *line_c,
                            const char *file_name) {
    regmatch_t match;
    int reti = regexec(regex, input, 1, &match, 0);

    flag_v(obj, &reti);

    if (!reti) {
        if (flag_c(obj, line_c, 0, file_name) == 0) return;
        int updateble = 1;
        int pattern_index = match.rm_so;
        int pattern_end = match.rm_eo - 1;
        take_correct_edges(input, &pattern_end, &pattern_index);

        int now_pattern_start = pattern_index;
        int now_pattern_end = pattern_end;

        for (int i = 0; input[i] != '\0'; i++) {
            print_now_file(file_name, input[i - 1] || '\0', i, obj);
            flag_n(line, obj, input[i - 1] || '\0', i, 0);
            print_char_in_infinuty(&pattern_index, &pattern_end, &updateble, input, &reti, regex, i, obj,
                                   line, match, &now_pattern_start, &now_pattern_end, file_name);
        }
        printf("\n");
    }
}

void while_decompose(int *line, char *input, regex_t *regex, MyObject *obj, const char *file_name,
                     int *line_c) {
    *line += 1;
    input[strcspn(input, "\n")] = '\0';  // Удаляем символ новой строки

    check_line_for_pattern(input, regex, *line, obj, line_c, file_name);
}

void infinity_input(regex_t *regex, MyObject *obj, const int sizeof_string, FILE *now_file,
                    const char *file_name) {
    if (flag_l(obj) == 0) return;

    int line = 0;
    int line_c = 0;
    char input[sizeof_string];

    if (now_file != NULL)
        while (fgets(input, sizeof(input), now_file) != NULL)
            while_decompose(&line, input, regex, obj, file_name, &line_c);

    else
        while (fgets(input, sizeof(input), stdin) != NULL)
            while_decompose(&line, input, regex, obj, file_name, &line_c);

    flag_c(obj, &line_c, 1, file_name);
}