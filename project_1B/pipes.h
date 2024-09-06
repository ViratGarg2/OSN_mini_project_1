#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "color.h"

char **split_command(char *cmd);
void handle_redirection(char **args);
void execute_pipe(char * str);