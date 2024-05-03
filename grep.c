// #include <ctype.h>
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

void check_opt(MyObject *obj, int argc, char *argv[], FILE *reg_ex_file, char *pattern);

void infinity_input(regex_t *regex, MyObject *obj);

int main(int argc, char *argv[]) {
    MyObject obj = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    FILE *reg_ex_file = NULL;
    char *pattern = malloc(sizeof(char) * 128);
    check_opt(&obj, argc, argv, reg_ex_file, pattern);

    // Проверяем, что передан патерн
    if (optind == argc && obj.f == 0) {
        printf("Usage: grep [OPTION]... PATTERNS [FILE]...");
        return 0;
    }
    if (obj.f == 0) pattern = argv[optind];

    regex_t regex;
    regcomp(&regex, pattern, REG_EXTENDED);

    if ((optind + 1 == argc && obj.f == 0) || (optind == argc && obj.f == 1)) infinity_input(&regex, &obj);

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

void flag_f(MyObject *obj, FILE *reg_ex_file, char *pattern) {
    obj->f = 1;
    reg_ex_file = fopen(optarg, "r");

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
};

void check_opt(MyObject *obj, int argc, char *argv[], FILE *reg_ex_file, char *pattern) {
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
                flag_f(obj, reg_ex_file, pattern);
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
}
void flag_n(int line, MyObject *obj, char prev_char, int i,
            int should) {  // should - принуждение для срабатывания, например с флагом o
    if (obj->n == 1 && (should || i == 0 || prev_char == '\n')) {
        if (isatty(fileno(stdout)))
            printf("\033[0;32m%d\033[0m\033[0;36m:\033[0m", line);
        else
            printf("%c:", line);
    }
}

void flag_o(int line, MyObject *obj, char prev_char, int i, int updateble) {
    if (obj->o == 1 && updateble) {
        printf("\n");
        flag_n(line, obj, prev_char, i, 1);
    }
}

void next_found(char *input, regex_t *regex, int *pattern_index, int *reti, int *pattern_end, int *updateble,
                regmatch_t match, int line, MyObject *obj, int i) {
    int tmp = *pattern_index;
    *reti = regexec(regex, input + i - 1, 1, &match, 0);
    *pattern_index = match.rm_so + i - 1;

    if (tmp == *pattern_index || *reti) *updateble = 0;
    *pattern_end = match.rm_eo + i - 1;

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

void check_line_for_pattern(char *input, regex_t *regex, int line, MyObject *obj) {
    regmatch_t match;
    int reti = regexec(regex, input, 1, &match, 0);

    if (!reti) {
        int updateble = 1;
        int pattern_index = match.rm_so;
        int pattern_end = match.rm_eo;

        for (int i = 0; input[i] != '\0'; i++) {
            flag_n(line, obj, input[i - 1] || '\0', i, 0);
            print_char_in_infinuty(&pattern_index, &pattern_end, &updateble, input, &reti, regex, i, obj,
                                   line, match);
        }
        printf("\n");
    }
}

void infinity_input(regex_t *regex, MyObject *obj) {  // ему безразличны флаги h, s
    int line = 0;
    char input[8192];
    while (fgets(input, sizeof(input), stdin) != NULL) {
        line++;
        input[strcspn(input, "\n")] = '\0';  // Удаляем символ новой строки

        check_line_for_pattern(input, regex, line, obj);
    }
}