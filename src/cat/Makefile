CC=gcc
CFLAGS=-Wall -Werror -Wextra -std=c11

SOURCES=s21_cat.c

all: s21_cat tests

s21_cat: clean
	$(CC) $(CFLAGS) $(SOURCES) -o s21_cat

tests:
	sh cat_tests.sh

clean:
	rm -f *.o s21_cat