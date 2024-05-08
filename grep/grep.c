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
#include "grep_infunity.c"

int check_opt(MyObject *obj, int argc, char *argv[], FILE *reg_ex_file, char *pattern);

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

    // flag_i(&obj, pattern);

    regex_t regex;
    if (obj.i == 1)
        regcomp(&regex, pattern, REG_ICASE);
    else
        regcomp(&regex, pattern, REG_EXTENDED);

    if ((optind + 1 == argc && obj.f == 0) || (optind == argc && obj.f == 1)) {
        infinity_input(&regex, &obj);
        regfree(&regex);
        if (pattern) free(pattern);
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
