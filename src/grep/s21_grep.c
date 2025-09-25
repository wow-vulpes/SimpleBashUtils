#include "s21_grep.h"

int main(int argc, char* argv[]) {
  if (argc > 1) {
    struct GrepOptions opt = {};
    struct Patterns patterns;
    patterns.size = 1;
    patterns.array = malloc(sizeof(regex_t) * patterns.size);
    patterns.count = 0;
    if (patterns.array != NULL) {
      if (OptionProcessing(argc, argv, &opt, &patterns)) {
        FindPatterns(argc, argv, opt, &patterns);
      } else {
        fprintf(
            stderr,
            "Usage: grep [OPTION]... PATTERNS [FILE]...\nTry not to cry.\n");
      }
      FreePatterns(&patterns);
      free(patterns.array);
    }
  }
  return 0;
}

int OptionProcessing(int argc, char* argv[], struct GrepOptions* opt,
                     struct Patterns* patterns) {
  int flag = 1;
  int symbol;
  while ((symbol = getopt(argc, argv, "e:ivclnhsf:o")) != -1 && flag) {
    switch (symbol) {
      case 'e':
        opt->e = 1;
        flag = Compile(patterns, optarg, *opt);
        break;

      case 'i':
        opt->i = 1;
        break;

      case 'v':
        opt->v = 1;
        break;

      case 'c':
        opt->c = 1;
        break;

      case 'l':
        opt->l = 1;
        break;

      case 'n':
        opt->n = 1;
        break;

      case 'h':
        opt->h = 1;
        break;

      case 's':
        opt->s = 1;
        break;

      case 'f':
        opt->f = 1;
        flag = OptionFCompile(patterns, *opt);
        break;

      case 'o':
        opt->o = 1;
        break;

      case '?':
      default:
        flag = 0;
    }
  }

  if (!(opt->e || opt->f)) {
    flag = Compile(patterns, argv[optind], *opt);
    ++optind;
  }

  if (optind + 1 == argc) {
    opt->h = 1;
  }

  return flag;
}

int Compile(struct Patterns* patterns, char* re, struct GrepOptions opt) {
  int flag = 1;
  regex_t pattern;

  if (patterns->count) {
    ++patterns->size;

    regex_t* reserve_array = patterns->array;

    if ((patterns->array = realloc(patterns->array,
                                   sizeof(regex_t) * patterns->size)) == NULL) {
      patterns->array = reserve_array;
      --patterns->size;
      flag = 0;
    }
  }

  if (!regcomp(&pattern, re, opt.i ? REG_ICASE : 0) && flag) {
    patterns->array[patterns->count] = pattern;
    ++patterns->count;
  } else {
    flag = 0;
  }

  return flag;
}

int OptionFCompile(struct Patterns* patterns, struct GrepOptions opt) {
  int flag = 0;
  FILE* file = fopen(optarg, "r");
  if (file != NULL) {
    ssize_t size;
    char* line = NULL;
    size_t n = 0;
    while ((size = getline(&line, &n, file)) != -1) {
      if (line[size - 1] == '\n') {
        line[size - 1] = '\0';
      }
      flag = Compile(patterns, line, opt);
    }
  }
  return flag;
}

void FindPatterns(int argc, char* argv[], struct GrepOptions opt,
                  struct Patterns* patterns) {
  for (int i = optind; i < argc; i++) {
    FILE* file = fopen(argv[i], "r");
    if (file != NULL) {
      ssize_t size;
      char* line = NULL;
      size_t n = 0;

      int count_of_matches = 0;
      int count_of_lines = 0;
      while ((size = getline(&line, &n, file)) != -1) {
        ++count_of_lines;
        RegexFind(*patterns, opt, argv[i], line, &count_of_matches,
                  count_of_lines);
      }

      if (opt.c && !opt.l) {
        if (!opt.h) {
          printf("%s:", argv[i]);
        }
        printf("%d\n", count_of_matches);
      }

      if (opt.l) {
        if (count_of_matches > 0) {
          printf("%s\n", argv[i]);
        }
      }

      free(line);

      fclose(file);
    } else {
      if (!opt.s) {
        fprintf(stderr, "s21_grep: %s: No such file or directory\n", argv[i]);
      }
    }
  }
}

void RegexFind(struct Patterns patterns, struct GrepOptions opt, char* filename,
               char* line, int* count_of_matches, int count_of_lines) {
  regmatch_t match[1];

  int count_of_not_match = 0;

  for (int i = 0; i < patterns.count; i++) {
    if (!regexec(patterns.array + i, line, 1, match, 0) && !opt.v) {
      if (opt.c || opt.l) {
        ++(*count_of_matches);
        break;
      }
      if ((opt.e || opt.f || opt.h || opt.i) && !opt.o) {
        PrintForOptions(opt, count_of_lines, filename);
        Print(line);
        break;
      } else if (!(opt.o || opt.v || opt.c || opt.l)) {
        PrintForOptions(opt, count_of_lines, filename);
        Print(line);
      }
    } else if (regexec(patterns.array + i, line, 1, match, 0) && opt.v &&
               !opt.o) {
      if (opt.c || opt.l) {
        ++(*count_of_matches);
        break;
      }
      ++count_of_not_match;
      if (count_of_not_match == patterns.count) {
        PrintForOptions(opt, count_of_lines, filename);
        Print(line);
      }
    }
  }

  if (opt.o && !opt.v && !opt.c && !opt.l) {
    int offset = 0;
    while (1) {
      regmatch_t first_match = {-1, -1};
      for (int i = 0; i < patterns.count; i++) {
        if (!regexec(patterns.array + i, (line + offset), 1, match, 0)) {
          if (first_match.rm_so == -1 || (match[0].rm_so < first_match.rm_so)) {
            first_match = match[0];
          }
        }
      }

      if (first_match.rm_so != -1) {
        PrintForOptions(opt, count_of_lines, filename);
        printf("%.*s\n", (int)(first_match.rm_eo - first_match.rm_so),
               line + (first_match.rm_so + offset));

        offset += first_match.rm_eo;
      } else {
        break;
      }
    }
  }
}

void PrintForOptions(struct GrepOptions opt, int count_of_lines,
                     char* filename) {
  if (!opt.h) {
    printf("%s:", filename);
  }

  if (opt.n) {
    printf("%d:", count_of_lines);
  }
}

void Print(char* line) {
  if (line[strlen(line) - 1] == '\n') {
    line[strlen(line) - 1] = '\0';
  }
  printf("%s", line);
  printf("\n");
}

void FreePatterns(struct Patterns* patterns) {
  for (int i = 0; i < patterns->count; i++) {
    regfree(patterns->array + i);
  }
}