
all:
	gcc -Wall -Werror -Wextra cat.c -o s21_cat
	gcc -Wall -Werror -Wextra grep.c -o s21_grep

clear:
	rm -rf cat tmp.txt