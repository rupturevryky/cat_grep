
all:
	gcc -Wall -Werror -Wextra cat.c -o cat

clear:
	rm -rf cat tmp.txt