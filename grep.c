#include <ctype.h>
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

void check_opt(MyObject *obj, int argc, char *argv[]);

void infinity_input(char *pattern);

int main(int argc, char *argv[]) {
    MyObject obj = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    check_opt(&obj, argc, argv);

    // Проверяем, что передан патерн
    if (optind == argc) {
        printf("Usage: grep [OPTION]... PATTERNS [FILE]...");
        return 0;
    }
    char *pattern = argv[optind];

    if (optind + 1 == argc) infinity_input(pattern);

    return 0;
}

void check_opt(MyObject *obj, int argc, char *argv[]) {
    int option;

    while ((option = getopt(argc, argv, "eivclnhsfo")) != -1) {
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
                obj->f = 1;
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

char *next_found(char *input, char *pattern, int *pattern_index, char *found, int *pattern_end,
                 int *updateble, int pattern_len) {
    int tmp = *pattern_index;
    found = strstr(found + 1, pattern);
    *pattern_index = found - input;
    if (tmp == *pattern_index) {
        *updateble = 0;
        return found;
    }
    *pattern_end = *pattern_index + pattern_len - 1;
    return found;
}

void colorize_line(char *input, char *pattern, int pattern_len) {
    char *found = strstr(input, pattern);

    if (found != NULL) {
        int pattern_index = found - input;
        int updateble = 1;
        int pattern_end = pattern_index + pattern_len - 1;

        for (int i = 0; input[i] != '\0'; i++) {
            if (i >= pattern_index && i <= pattern_end)
                printf("\x1b[31m%c\x1b[0m", input[i]);
            else if (updateble && i > pattern_end && found != NULL) {
                found =
                    next_found(input, pattern, &pattern_index, found, &pattern_end, &updateble, pattern_len);
                i--;
                continue;
            } else
                printf("%c", input[i]);
        }
        printf("\n");
    }
}

void infinity_input(char *pattern) {
    char input[8192];
    int pattern_len = strlen(pattern);
    while (fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = '\0';  // Удаляем символ новой строки

        colorize_line(input, pattern, pattern_len);
    }
}