#include <unistd.h>
#include <wordexp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include "error.h"
#include "help.h"

#define MAX_IN_SIZE  1024
#define MAX_CMD_SIZE 4096
#define PLACEHOLDER  "%s"

int run_command(char *cmd);

int main(int argc, char *argv[]) {
  int c;
  while ((c = getopt(argc, argv, "h")) != -1 ) {
    switch (c) {
      case 'h':
        show_help();
        return 0;
    }
  }

  char input[MAX_IN_SIZE] = {0};
  char cmd[MAX_CMD_SIZE] = {0};

  if (!fgets(input, sizeof input, stdin))
    fatal("failed to read from stdin");

  /*
    fgets reads until \n, and preserves it so we clean it here
  */
  size_t len = strlen(input);
  if (len > 0 && input[len - 1] == '\n')
    input[len - 1] = '\0';

  /*
    if we do not have any argument, we want to run stdin as it
    TODO: do we?
  */
  if (argc == 1)
    return run_command(input);

  /*
    we want to build the cmd string, using argv values concatenated
  */
  for (int i = 1; i < argc; i++) {
    if (strlen(cmd) + strlen(argv[i]) + 2 >= MAX_CMD_SIZE)
      fatal("command too long");

    /*
      append argv[i] to the cmd being built, and if there're still other
      elements to append, then we add a whitespace to separate them
    */
    strcat(cmd, argv[i]);
    if (i + 1 < argc)
      strcat(cmd, " ");
  }

  char built_cmd[MAX_CMD_SIZE] = {0};
  char *p = strstr(cmd, PLACEHOLDER);

  /*
    if the placeholder string is not found, then simply execute what's in argv
  */
  if (!p)
    return run_command(cmd);

  size_t prefix = p - cmd;

  memcpy(built_cmd, cmd, prefix);
  built_cmd[prefix] = '\0';

  strcat(built_cmd, input);
  strcat(built_cmd, p + 2);

  return run_command(built_cmd);
}

int run_command(char *cmd) {
  pid_t pid = fork();
  if (pid < 0)
    fatal("parent process fork failed");

  if (pid  == 0) {
      /*
        as cmd is a single string, we need to properly split it to use
        exec()-like functions.
        to do so without breaking quoting, it's possible to use wordexp that is
        a sort of shell tokenizer
      */
      wordexp_t w;
      wordexp(cmd, &w, 0);
      execvp(w.we_wordv[0], w.we_wordv);
      fatal("child exec failed");
  }
  int status = 0;
  waitpid(pid, &status, 0);
  return status;
}
