#ifndef S21_GREP_H
#define S21_GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct GrepOptions {
  char e;
  char i;
  char v;
  char c;
  char l;
  char n;
  char h;
  char s;
  char f;
  char o;
};

struct Patterns {
  regex_t* array;
  int count;
  int size;
};

int OptionProcessing(int argc, char* argv[], struct GrepOptions* opt,
                     struct Patterns* patterns);

int Compile(struct Patterns* patterns, char* re, struct GrepOptions opt);

int OptionFCompile(struct Patterns* patterns, struct GrepOptions opt);

void FindPatterns(int argc, char* argv[], struct GrepOptions opt,
                  struct Patterns* patterns);

void RegexFind(struct Patterns patterns, struct GrepOptions opt, char* filename,
               char* line, int* count_of_matches, int count_of_lines);

void FreePatterns(struct Patterns* patterns);

void PrintForOptions(struct GrepOptions opt, int count_of_lines,
                     char* filename);

void Print(char* line);

#endif