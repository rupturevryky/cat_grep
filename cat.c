#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
    int b;
    int e;
    int n;
    int s;
    int t;
} MyObject;

void infinity_input(MyObject *obj);

void check_opt(MyObject *obj, int argc, char *argv[]);
void flag_b(const int cheker, int *line);

int is_string_empty(int i, char *buffer);
void if_start_string(int const start_string, MyObject *obj, int *line, int empty_string_checker, int flag_s,
                     char now_ch);

int main(int argc, char *argv[]) {
    MyObject obj = {0, 0, 0, 0, 0};

    // Проверяем, что переданы аргументы командной строки
    if (argc < 2) infinity_input(&obj);

    // считываем флаги и параметры
    check_opt(&obj, argc, argv);
    if (optind >= argc) infinity_input(&obj);

    // флаги для вывода
    int flag_s = 0;
    int empty_string_checker = 0;

    // флаг обозначения начала строки
    int start_string = 1;
    // Отслеживание текущей строки
    int line = 1;

    // бежит по файлам
    for (int i = optind; i < argc; i++) {
        // текущий файл
        FILE *now_file = fopen(argv[i], "r");
        // Проверяем, удалось ли открыть файл
        if (now_file == NULL) {
            printf("cat: %s: No such file or directory\n", argv[i]);
            continue;
        }
        // Определение размера файла
        fseek(now_file, 0, SEEK_END);
        long fileSize = ftell(now_file);
        fseek(now_file, 0, SEEK_SET);
        // Создаём массим buffer и копируем в него текущий файл
        char *buffer = (char *)malloc(fileSize);
        if (buffer == NULL) {
            fclose(now_file);
            continue;
        }
        fread(buffer, fileSize, 1, now_file);
        buffer[fileSize] = '\0';

        for (int i = 0; i < fileSize; i++) {
            if (i > 0 && buffer[i - 1] != '\n')
                start_string = 0;
            else
                start_string = 1;

            // Проверка на пустоту для флага -s (--squeeze-blank) и -b
            if ((obj.b == 1 || obj.s == 1) && start_string)
                empty_string_checker = is_string_empty(i, buffer);
            else
                empty_string_checker = 0;
            if (obj.s == 1 && empty_string_checker == 1) {
                flag_s += 1;
            }
            if (empty_string_checker == 0) flag_s = 0;
            if (flag_s > 1) continue;

            // Конец строки, обработка флага -n
            if (buffer[i] == '\n') {
                if (obj.n == 1) line++;
                // Начало строки -n и -b
                if_start_string(start_string, &obj, &line, empty_string_checker, flag_s, buffer[i]);
                printf("%c", buffer[i]);

                continue;
            }

            // Начало строки -n и -b
            if_start_string(start_string, &obj, &line, empty_string_checker, flag_s, buffer[i]);

            // флаг -t
            if (obj.t == 1 && buffer[i] == '\t')
                printf("^I");
            else
                printf("%c", buffer[i]);

            // флаг -e
            if (obj.e == 1 && buffer[i + 1] == '\n' && (obj.s == 0 || !empty_string_checker || flag_s <= 1))
                printf("$");

            // Флаг -e, когда файл закончился
            if (obj.e == 1 && (i + 1) == fileSize) printf("$");
        }
        free(buffer);
        if (now_file) fclose(now_file);
    }
    return 0;
}

int is_string_empty(int i, char *buffer) {
    if (buffer[i] == '\n') return 1;
    return 0;
}
void check_opt(MyObject *obj, int argc, char *argv[]) {
    int option;

    while ((option = getopt(argc, argv, "benst")) != -1) {
        switch (option) {
            case 'b':
                obj->b = 1;
                obj->n = 0;
                break;
            case 'n':
                if (!obj->b) obj->n = 1;
                break;
            case 'e':
                obj->e = 1;
                break;
            case 's':
                obj->s = 1;
                break;
            case 't':
                obj->t = 1;
                break;
            case '?':
                exit(1);
            default:
                abort();
        }
    }
}

void flag_b(const int empty_cheker, int *line) {
    if (!empty_cheker) {
        printf("%6d\t", *line);
        *line += 1;
    }
}

void if_start_string(int const start_string, MyObject *obj, int *line, int empty_string_checker, int flag_s,
                     char now_ch) {
    if (start_string) {
        if (obj->n == 1) printf("%6d\t", *line);
        if (obj->b == 1) flag_b(empty_string_checker, line);
        // флаг -e
        if (now_ch == '\n' && obj->e == 1 && (obj->s == 0 || !empty_string_checker || flag_s <= 1))
            printf("$");
    }
}

void print_char_inf(char *input, int t) {
    if (strcmp(input, "\n") != 0) {
        for (int i = 0; input[i] != '\n'; i++) {
            // флаг -t
            if (t == 1 && input[i] == '\t')
                printf("^I");
            else
                printf("%c", input[i]);
        }
    }
}

void infinity_input(MyObject *obj) {
    char input[4096];
    int line = 1;
    int flag_s = 0;
    while (1) {
        fgets(input, sizeof(input), stdin);

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

        print_char_inf(input, obj->t);

        if (obj->e == 1) printf("$");
        printf("\n");

        if (obj->s == 1 && empty_string_checker) flag_s++;
    }
}