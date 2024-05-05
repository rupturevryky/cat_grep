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

void flag_i(MyObject *obj, char *str);

int check_opt(MyObject *obj, int argc, char *argv[], FILE *reg_ex_file, char *pattern);

void infinity_input(regex_t *regex, MyObject *obj);

int main(int argc, char *argv[]) {
    MyObject obj = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    FILE *reg_ex_file = NULL;
    char *pattern = malloc(sizeof(char) * 128);

    if (check_opt(&obj, argc, argv, reg_ex_file, pattern) != 0) {
        free(pattern);
        return 1;
    };
    // Проверяем, что передан патерн
    if (optind == argc && obj.f == 0) {
        printf("Usage: grep [OPTION]... PATTERNS [FILE]...");
        free(pattern);
        return 1;
    }

    if (obj.f == 0) pattern = argv[optind];

    flag_i(&obj, pattern);

    regex_t regex;
    if (obj.i == 1)
        regcomp(&regex, pattern, REG_EXTENDED | REG_ICASE);
    else
        regcomp(&regex, pattern, REG_EXTENDED);

    if ((optind + 1 == argc && obj.f == 0) || (optind == argc && obj.f == 1)) {
        infinity_input(&regex, &obj);

        // if (pattern) free(pattern);
        return 0;
    }
    optind++;

    FILE *file = NULL;
    while (optind < argc) {
        char *filename = argv[optind];
        file = fopen(filename, "r");

        if (file == NULL) {
            printf("grep: %s: No such file or directory", filename);
        };
        optind++;
    }
    return 0;
}

void flag_i(MyObject *obj, char *str) {
    if (obj->i == 1) {
        int i = 0;
        while (str[i]) {
            str[i] = tolower(str[i]);
            i++;
        }
    }
}

int flag_f(MyObject *obj, FILE *reg_ex_file, char *pattern) {
    obj->f = 1;
    reg_ex_file = fopen(optarg, "r");

    if (reg_ex_file != NULL) {
        long file_size;
        // Определяем размер файла
        fseek(reg_ex_file, 0, SEEK_END);
        file_size = ftell(reg_ex_file);
        rewind(reg_ex_file);

        // Выделяем память для буфера
        pattern = (char *)realloc(pattern, file_size * sizeof(char));
        // Читаем содержимое файла в буфер
        fread(pattern, sizeof(char), file_size, reg_ex_file);

        // Закрываем файл
        fclose(reg_ex_file);
    } else {
        printf("grep: %s: No such file or directory", optarg);
        return 1;
    }
    return 0;
};

void flag_n(int line, MyObject *obj, char prev_char, int i,
            int should) {  // should - принуждение для срабатывания, например с флагом o
    if (obj->n == 1 && (should || i == 0 || prev_char == '\n')) {
        if (isatty(fileno(stdout)))
            printf("\033[0;32m%d\033[0m\033[0;36m:\033[0m", line);
        else
            printf("%d:", line);
    }
}

void flag_o(int line, MyObject *obj, char prev_char, int i, int updateble) {
    if (obj->o == 1 && updateble) {
        printf("\n");
        flag_n(line, obj, prev_char, i, 1);
    }
}

int flag_c(MyObject *obj, int *line_c, int end) {
    if (obj->c == 1) {
        if (end) {  // скипать больше некуда
            printf("%d", *line_c);
            return 0;
        }
        // obj->c == 1 - скипаем дальше считаем линии
        *line_c += 1;
        return 0;
    }
    return 1;  // obj->c == 0 - не скипаем, печатаем основной функционал grep
}

int flag_l(MyObject *obj) {
    if (obj->l == 1) {
        if (isatty(fileno(stdout)))
            printf("\033[38;5;129m(standard input)\033[0m");
        else
            printf("(standard input)");
        return 0;
    }
    return 1;
}

void flag_v(MyObject *obj, int *reti) {
    if (obj->v == 1) *reti = !*reti;
}

int check_opt(MyObject *obj, int argc, char *argv[], FILE *reg_ex_file, char *pattern) {
    int option;
    while ((option = getopt(argc, argv, "eivclnhsof:")) != -1) {
        switch (option) {
            case 'e':
                obj->e = 1;
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
                break;
            case 'n':
                obj->n = 1;
                break;
            case 'h':
                obj->h = 1;
                break;
            case 's':
                obj->s = 1;
                break;
            case 'f':
                if (flag_f(obj, reg_ex_file, pattern) != 0) return 1;
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

// void take_true_end(char *input, int *pattern_end) {
//     // printf("\npattern_end before: %d\n", *pattern_end);
//     if (!isalpha(input[*pattern_end])) {
//         //  !isalpha(input[*pattern_end + 1] && !isalpha(input[*pattern_end + 2]))))
//         *pattern_end += 1;
//         *pattern_end -= 1;
//     }
// }

void next_found(char *input, regex_t *regex, int *pattern_index, int *reti, int *pattern_end, int *updateble,
                regmatch_t match, int line, MyObject *obj, int i) {
    int tmp = *pattern_index;
    *reti = regexec(regex, input + i - 1, 1, &match, 0);
    *pattern_index = match.rm_so + i - 1;

    if (tmp == *pattern_index || *reti)
        *updateble = 0;
    else {
        *pattern_end = match.rm_eo + i - 1;
        // take_true_end(input, pattern_end, *pattern_index);
        // take_true_end(input, pattern_end);
    }

    flag_o(line, obj, input[i - 1] || '\0', i, *updateble);
}

void print_char_in_infinuty(int *pattern_index, int *pattern_end, int *updateble, char *input, int *reti,
                            regex_t *regex, int i, MyObject *obj, int line, regmatch_t match) {
    if (*updateble && i > *pattern_end) {
        next_found(input, regex, pattern_index, reti, pattern_end, updateble, match, line, obj, i);
    }
    if (i >= *pattern_index && i < *pattern_end) {
        if (isatty(fileno(stdout))) {
            if (i == *pattern_index) printf("\033[1;31m");
            printf("%c", input[i]);
            if (i == *pattern_end - 1) printf("\033[0m");
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
        int pattern_end = match.rm_eo;
        // take_true_end(input, &pattern_end);

        for (int i = 0; input[i] != '\0'; i++) {
            flag_n(line, obj, input[i - 1] || '\0', i, 0);
            print_char_in_infinuty(&pattern_index, &pattern_end, &updateble, input, &reti, regex, i, obj,
                                   line, match);
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
        flag_i(obj, input);
        line++;
        input[strcspn(input, "\n")] = '\0';  // Удаляем символ новой строки

        check_line_for_pattern(input, regex, line, obj, &line_c);
    }
    flag_c(obj, &line_c, 1);
}