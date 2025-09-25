#include "s21_cat.h"

int main(int argc, char* argv[]) {
  if (argc > 1) {
    struct CatOptions opt = {};
    if (GetOptions(argc, argv, &opt)) {
      WorkWithFiles(argc, argv, opt);
    } else {
      fprintf(stderr, "Try not to cry.\n");
    }
  }

  return 0;
}

int GetOptions(int argc, char* argv[], struct CatOptions* opt) {
  int flag = 1;
  const char* short_options = "benstvET";
  const struct option long_options[] = {
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", no_argument, NULL, 'n'},
      {"squeeze-blank", no_argument, NULL, 's'},
      {NULL, 0, NULL, 0}};

  int symbol;
  while ((symbol = getopt_long(argc, argv, short_options, long_options,
                               NULL)) != -1) {
    switch (symbol) {
      case 'b':
        opt->b = 1;
        break;

      case 'v':
        opt->v = 1;
        break;

      case 'e':
        opt->v = 1;
        opt->e = 1;
        break;
      case 'E':
        opt->e = 1;
        break;

      case 'n':
        opt->n = 1;
        break;

      case 's':
        opt->s = 1;
        break;

      case 't':
        opt->v = 1;
        opt->t = 1;
        break;
      case 'T':
        opt->t = 1;
        break;

      case '?':
      default:
        flag = 0;
        break;
    }
  }
  if (opt->b == 1) {
    opt->n = 0;
  }
  return flag;
}

void WorkWithFiles(int argc, char* argv[], const struct CatOptions opt) {
  int number = 1;
  int switch_s = 1;
  int new_file = 1;
  for (int i = optind; i < argc; i++) {
    FILE* file = fopen(argv[i], "r");

    if (file == NULL) {
      fprintf(stderr, "s21_cat: %s: No such file or directory\n", argv[i]);
      continue;
    }

    if (i != optind) {
      new_file = 0;
    }

    for (int symbol = fgetc(file), previous_symbol = 0; symbol != EOF;
         symbol = fgetc(file)) {
      if (new_file) {
        if (opt.b || opt.n) {
          OptionBN(&number, symbol, previous_symbol, opt);
        }
      }
      new_file = 1;
      if (opt.s) {
        OptionS(symbol, previous_symbol, &switch_s);
      }

      if (opt.v) {
        OptionV(&symbol);
      }

      if (symbol == '\t' && opt.t) {
        printf("^|");
      } else if (symbol == '\n' && opt.e) {
        printf("$\n");
      } else if (switch_s) {
        printf("%c", symbol);
      }

      previous_symbol = symbol;
    }
    fclose(file);
  }
}

void OptionBN(int* number, char symbol, char previous_symbol,
              struct CatOptions opt) {
  if (!(opt.b && symbol == '\n') &&
      (previous_symbol == '\n' || previous_symbol == 0)) {
    printf("%6d	", *number);
    (*number)++;
  }
}

void OptionS(char symbol, char previous_symbol, int* switch_s) {
  if ((previous_symbol == '\n' || previous_symbol == 0) && symbol == '\n') {
    if (*switch_s) {
      printf("\n");
    }
    *switch_s = 0;

  } else {
    *switch_s = 1;
  }
}

void OptionV(int* symbol) {
  if (*symbol != '\n' && *symbol != '\t') {
    if (*symbol > 127) {
      *symbol -= 128;
      printf("M-");
    }
    if (*symbol < 32) {
      printf("^");
      *symbol += 64;
    } else if (*symbol == 127) {
      printf("^");
      *symbol -= 64;
    }
  }
}
