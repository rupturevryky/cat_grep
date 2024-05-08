
all:
	gcc -Wall -Werror -Wextra ./cat/cat.c -o ./cat/s21_cat
	gcc -Wall -Werror -Wextra ./grep/grep.c -o ./grep/s21_grep

cat:
	gcc -Wall -Werror -Wextra ./cat/cat.c -o ./cat/s21_cat

grep:
	gcc -Wall -Werror -Wextra ./grep/grep.c -o ./grep/s21_grep

clear:
	rm -rf cat tmp.txt