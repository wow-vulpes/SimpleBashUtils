CC=gcc
CFLAGS=-Wall -Werror -Wextra -D_POSIX_C_SOURCE=201701 -std=c11

SOURCES=s21_grep.c

all: s21_grep tests

s21_grep: clean
	$(CC) $(CFLAGS) $(SOURCES) -o s21_grep

tests:
	sh grep_tests.sh

clean:
	rm -f *.o s21_grep