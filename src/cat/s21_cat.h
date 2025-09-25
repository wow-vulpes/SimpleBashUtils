#ifndef S21_CAT_H
#define S21_CAT_h

#include <getopt.h>
#include <stdio.h>

struct CatOptions {
  char b;
  char e;
  char n;
  char s;
  char t;
  char v;
};

int GetOptions(int argc, char* argv[], struct CatOptions* opt);

void WorkWithFiles(int argc, char* argv[], const struct CatOptions opt);

void OptionBN(int* number, char symbol, char previous_symbol,
              struct CatOptions opt);

void OptionS(char symbol, char previous_symbol, int* flag_for_s);

void OptionV(int* symbol);

#endif