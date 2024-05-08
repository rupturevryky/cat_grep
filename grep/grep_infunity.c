

int take_correct_edges(char *input, int *pattern_end, int *pattern_index) {
    // printf("\nstart pattern: %d\n", *pattern_index);
    // printf("match.rm_eo start: %d\n", *pattern_end);
    if (isascii(input[*pattern_end])) return 0;

    float count_ru = 0;
    int count_else = 0;
    int end = *pattern_end;
    int i = *pattern_index;

    for (i = i; i <= end; i++) {
        if (!isascii(input[i])) {
            count_ru += 0.5;
        } else
            count_else++;
    }

    if (count_ru != (int)count_ru) {
        count_ru += 0.5;
        end++;
    }

    int start_len = *pattern_end - *pattern_index + 1;
    float need_len = count_ru * 2 + count_else;
    if (start_len == need_len) return 0;
    int res = count_ru;
    count_ru = 0;
    int flag_to_pull_ru = 0;
    for (i = i; i <= end; i++) {
        if (!isascii(input[i])) {
            count_ru += 0.5;
            flag_to_pull_ru++;
            if (flag_to_pull_ru >= 2) {
                end++;
                flag_to_pull_ru = 0;
            }
        }
    }

    if (*pattern_end < end) *pattern_end = end;
    // printf("pattern_end: %d\n\n", *pattern_end);
    return res;
}

void next_found(char *input, regex_t *regex, int *pattern_index, int *reti, int *pattern_end, int *updateble,
                regmatch_t match, int line, MyObject *obj, int i, int *was_ru_char) {
    int tmp = *pattern_index;
    *reti = regexec(regex, input + i - 1, 1, &match, 0);
    *pattern_index = match.rm_so + i - 1;

    if (tmp == *pattern_index || *reti) {
        *updateble = 0;
    } else {
        if (!isascii(input[match.rm_eo + i - 1]) || *was_ru_char % 2 == 0)
            *pattern_end = match.rm_eo + i - 2;
        else
            *pattern_end = match.rm_eo + i - 1;
        *was_ru_char += take_correct_edges(input, pattern_end, pattern_index);
    }

    flag_o(line, obj, input[i - 1] || '\0', i, *updateble);
}

void print_char_in_infinuty(int *pattern_index, int *pattern_end, int *updateble, char *input, int *reti,
                            regex_t *regex, int i, MyObject *obj, int line, regmatch_t match,
                            int *was_ru_char) {
    if (*updateble && i > *pattern_end) {
        next_found(input, regex, pattern_index, reti, pattern_end, updateble, match, line, obj, i,
                   was_ru_char);
    }
    if (i >= *pattern_index && i <= *pattern_end) {
        if (isatty(fileno(stdout))) {
            if (i == *pattern_index) printf("\033[1;31m");
            printf("%c", input[i]);
            if (i == *pattern_end) printf("\033[0m");
        } else
            printf("%c", input[i]);
    } else if (obj->o == 0)
        printf("%c", input[i]);
};

void check_line_for_pattern(char *input, regex_t *regex, int line, MyObject *obj, int *line_c) {
    regmatch_t match;
    int reti = regexec(regex, input, 1, &match, 0);

    flag_v(obj, &reti);

    if (!reti) {
        if (flag_c(obj, line_c, 0) == 0) return;
        int updateble = 1;
        int pattern_index = match.rm_so;
        int pattern_end = match.rm_eo - 1;
        int was_ru_char = 0;
        was_ru_char += take_correct_edges(input, &pattern_end, &pattern_index);

        for (int i = 0; input[i] != '\0'; i++) {
            flag_n(line, obj, input[i - 1] || '\0', i, 0);
            print_char_in_infinuty(&pattern_index, &pattern_end, &updateble, input, &reti, regex, i, obj,
                                   line, match, &was_ru_char);
        }
        printf("\n");
    }
}

void infinity_input(regex_t *regex, MyObject *obj) {  // ему безразличны флаги h, s

    if (flag_l(obj) == 0) return;

    int line = 0;
    int line_c = 0;
    char input[8192];
    while (fgets(input, sizeof(input), stdin) != NULL) {
        // flag_i(obj, input);
        line++;
        input[strcspn(input, "\n")] = '\0';  // Удаляем символ новой строки

        check_line_for_pattern(input, regex, line, obj, &line_c);
    }
    flag_c(obj, &line_c, 1);
}