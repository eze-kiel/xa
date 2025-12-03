#include <stdio.h>
#include <stdlib.h>

void fatal(char *msg) {
  fprintf(stderr, "fatal: %s\n", msg);
  exit(1);
}
