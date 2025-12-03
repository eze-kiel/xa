#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_IN_SIZE  1024
#define MAX_CMD_SIZE 4096

int run_command(char *cmd);

int main(int argc, char *argv[]) {
  char input[MAX_IN_SIZE] = {0};
  char cmd[MAX_CMD_SIZE] = {0};

  if (!fgets(input, sizeof input, stdin))
    return 1;

  /*
    fgets reads until \n, and preserves it so we clean it here
  */
  size_t len = strlen(input);
  if (len > 0 && input[len - 1] == '\n')
    input[len - 1] = '\0';

  /*
    if we do not have any argument, we want to run stdin as it
  */
  if (argc == 1)
    return run_command(input);

  /*
    we want to build the cmd string, using argv values concatenated
  */
  for (int i = 1; i < argc; i++) {
    if (strlen(cmd) + strlen(argv[i]) + 2 >= MAX_CMD_SIZE) {
      fprintf(stderr, "err: command too long\n");
      return 1;
    }

    /*
      append argv[i] to the cmd being built, and if there're still other
      elements to append, then we add a whitespace to separate them
    */
    strcat(cmd, argv[i]);
    if (i + 1 < argc)
      strcat(cmd, " ");
  }

  char built_cmd[MAX_CMD_SIZE] = {0};
  char *p = strstr(cmd, "%s");

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
  int status = system(cmd);
  if (WIFEXITED(status))
    return WEXITSTATUS(status);
  return 1;
}
