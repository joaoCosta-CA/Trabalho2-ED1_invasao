#include "argumentos.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

char *get_option_value(int argc, char *argv[], char *opt_name) {
    for (int i = 1; i < argc - 1; ++i) {
        if (argv[i][0] == '-' && strcmp(argv[i] + 1, opt_name) == 0) {
            return argv[i + 1];
        }
    }
    return NULL;
}

char *get_command_suffix(int argc, char *argv[]) {
    char **argv_copy = malloc(argc * sizeof(char *));
    if (argv_copy == NULL)
      return NULL;
  
    for (int i = 0; i < argc; i++) {
      argv_copy[i] = argv[i];
    }
  
    int new_argc = argc;
  
    for (int i = 1; i < new_argc - 1; i++) {
      if (argv_copy[i][0] == '-') {
        if (argv_copy[i + 1][0] != '-') {
          for (int j = i; j < new_argc - 2; j++) {
            argv_copy[j] = argv_copy[j + 2];
          }
          new_argc -= 2;
          i--; 
        }
      }
    }
  
    char *suffix = NULL;
    if (new_argc == 2) {
      suffix = argv_copy[new_argc - 1];
    } else if (new_argc > 2) {
      printf("Erro: Mais de um sufixo\n");
      free(argv_copy);
      exit(1);
    }
  
    free(argv_copy);
    return suffix;
  }


  int diretorio_valido(const char *path) {
    struct stat st = {0};
    if (stat(path, &st) == -1) return 0;
    return S_ISDIR(st.st_mode);
}