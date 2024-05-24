
all: s21_cat s21_grep

cat:
	gcc -Wall -Werror -Wextra ./cat/cat.c -o ./cat/$@

grep:
	gcc -Wall -Werror -Wextra ./grep/grep.c -o ./grep/$@

clean:
	rm -rf ./grep/cat ./cat/grep ./common/test_grep2.txt ./common/test_grep1.txt ./common/patern.txt ./common/test_cat1.txt ./common/test_cat2.txt