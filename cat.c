#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int b;
    int e;
    int n;
    int s;
    int t;
    int v;
} MyObject;

void infinity_input(MyObject *obj);

int check_flag(char *c, MyObject *obj, FILE *tmp);
void flag_b(const int cheker, int *line);

int is_string_empty(int i, char *buffer);

int main(int argc, char *argv[]) {
    MyObject obj = {0, 0, 0, 0, 0, 0};

    // Проверяем, что переданы аргументы командной строки
    if (argc < 2) {
        infinity_input(&obj);
    }
    char ch;

    // промежуточный файл
    FILE *tmp = fopen("tmp.txt", "a+");
    int count_of_files = 0;
    int is_tmp_pulled = 0;
    char last_char = '\0';

    // Перебираем все переданные аргументы
    for (int i = 1; i < argc; i++) {
        if (check_flag(argv[i], &obj, tmp)) continue;
        count_of_files++;

        // Открываем файл для чтения
        FILE *file;
        file = fopen(argv[i], "r");

        // Проверяем, удалось ли открыть файл
        if (file == NULL) {
            printf("cat: %s: No such file or directory\n", argv[i]);
            continue;
        }

        // записываем в tmp для конкатинации
        if (is_tmp_pulled && last_char != '\n') {
            fputc('\r', tmp);
            fputc('\n', tmp);
        }
        while ((ch = fgetc(file)) != EOF) {
            last_char = ch;
            fputc(ch, tmp);
        }
        // Закрываем файл
        is_tmp_pulled = 1;
        fclose(file);
    }
    if (!count_of_files) infinity_input(&obj);

    // Определение размера файла
    fseek(tmp, 0, SEEK_END);
    long fileSize = ftell(tmp);
    fseek(tmp, 0, SEEK_SET);

    // Выделение памяти под буфер
    char *buffer = (char *)malloc(fileSize);
    if (buffer == NULL) {
        printf("Ошибка выделения памяти");
        remove("tmp.txt");
        if (tmp) fclose(tmp);
        return 1;
    }
    // Запись в бувер данных из файла
    fread(buffer, fileSize, 1, tmp);
    buffer[fileSize] = '\0';

    int flag_s = 0;
    int empty_string_checker = 0;

    // флаг обозначения начала строки
    int start_string = 1;
    // Отслеживание текущей строки
    int line = 1;

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
            if (start_string) {
                if (obj.n == 1) printf("%6d\t", line);
                if (obj.b == 1) flag_b(empty_string_checker, &line);
                // флаг -e
                if (obj.e == 1 && (obj.s == 0 || !empty_string_checker || flag_s <= 1)) printf("$");
            }
            printf("%c", buffer[i]);

            continue;
        }

        // Начало строки -n и -b
        if (start_string) {
            if (obj.n == 1) printf("%6d\t", line);
            if (obj.b == 1) flag_b(empty_string_checker, &line);
        }

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
    remove("tmp.txt");
    if (tmp) fclose(tmp);

    return 0;
}

int is_string_empty(int i, char *buffer) {
    if (buffer[i] == '\n') return 1;
    return 0;
}
int check_flag(char *c, MyObject *obj, FILE *tmp) {
    if (c[0] == '-') {
        if (strcmp(c, "-b") == 0 || strcmp(c, "--number-nonblank") == 0) {
            obj->b = 1;
            obj->n = 0;
            return 1;
        }
        if (strcmp(c, "-e") == 0) {
            obj->e = 1;
            obj->v = 1;
            return 1;
        }
        if (strcmp(c, "-n") == 0) {
            if (!obj->b) obj->n = 1;
            return 1;
        }
        if (strcmp(c, "-s") == 0 || strcmp(c, "--squeeze-blank") == 0) {
            obj->s = 1;
            return 1;
        }
        if (strcmp(c, "-t") == 0) {
            obj->t = 1;
            obj->v = 1;
            return 1;
        }
        printf("cat: invalid option - '%s'\n", c);
        remove("tmp.txt");
        if (tmp) fclose(tmp);
        exit(1);
    }
    return 0;
}

void flag_b(const int empty_cheker, int *line) {
    if (!empty_cheker) {
        printf("%6d\t", *line);
        *line += 1;
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
            printf("    %d  ", line);
            line++;
        };
        if (obj->b == 1) flag_b(empty_string_checker, &line);
        if (strcmp(input, "\n") != 0) {
            for (int i = 0; input[i] != '\n'; i++) {
                // флаг -t
                if (obj->t == 1 && input[i] == '\t')
                    printf("^I");
                else
                    printf("%c", input[i]);
            }
        }
        if (obj->e == 1) printf("$");
        printf("\n");

        if (obj->s == 1 && empty_string_checker) flag_s++;
    }
}