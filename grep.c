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

void infinity_input();

int main(int argc, char *argv[]) {
    MyObject obj = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    check_opt(&obj, argc, argv);

    // Проверяем, что переданы аргументы командной строки
    if (optind >= argc) {
        printf("Usage: grep [OPTION]... PATTERNS [FILE]...");
        return 0;
    }
    infinity_input(&obj);

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

void infinity_input() {
    char input[8192];
    while (fgets(input, sizeof(input), stdin) != NULL) {
        printf("\x1b[31m%s\x1b[0m", input);
    }
}