#include <stdio.h>

void show_help() {
  printf("xa - like xargs, but worse\n\n");
  printf("Options:\n");
  printf("    -h\n        Show this help\n");
  printf("Usage:\n");
  printf("    echo /etc/passwd | xa ls %%s\n");
}
