
all:
	gcc -Wall -Werror -Wextra ./cat/cat.c -o ./cat/s21_cat
	gcc -Wall -Werror -Wextra ./grep/grep.c -o ./grep/s21_grep

cat:
	gcc -Wall -Werror -Wextra ./cat/cat.c -o ./cat/s21_cat

grep:
	gcc -Wall -Werror -Wextra ./grep/grep.c -o ./grep/s21_grep

clean:
	rm -rf ./grep/s21_grep ./cat/s21_cat ./common/test_grep2.txt ./common/test_grep1.txt ./common/patern.txt ./common/test_cat1.txt ./common/test_cat2.txt