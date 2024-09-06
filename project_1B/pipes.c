#include "pipes.h"
#define MAX_CMDS 10
#define MAX_ARGS 10

void handle_redirection(char **args) {
    int i = 0;

    while (args[i] != NULL) {
        if (strcmp(args[i], ">") == 0 || strcmp(args[i], ">>") == 0) {
            // Output redirection
            int flags = O_WRONLY | O_CREAT | (strcmp(args[i], ">>") == 0 ? O_APPEND : O_TRUNC);
            int fd = open(args[i + 1], flags, 0644);
            if (fd == -1) {
                perror("open");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
            args[i] = NULL;  // Remove redirection operator
            args[i + 1] = NULL;  // Remove file name
        }
       else if (strcmp(args[i], "<") == 0) {
            // Input redirection
            int fd = open(args[i + 1], O_RDONLY);
            if (fd == -1) {
                perror("open");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
            args[i] = NULL;  // Remove redirection operator
            args[i + 1] = NULL;  // Remove file name
        }
        i++;
    }
}

// Simulate splitting command into arguments (like shell's parsing)
char **split_command(char *command) {
    char **args = malloc(64 * sizeof(char *));
    char *arg;
    int i = 0;

    arg = strtok(command, " \t\n");
    while (arg != NULL) {
        args[i++] = arg;
        arg = strtok(NULL, " \t\n");
    }
    args[i] = NULL;
    return args;
}

// Function to handle pipelines and redirection
void execute_pipe(char *input) {
    char *commands[MAX_CMDS];
    int num_cmds = 0;
    int pipefd[2];
    int input_fd = 0;  // Initially, standard input (stdin)
    pid_t pid;
   // printf("%s",input);
    // Split input string into commands using '|'
    char *token = strtok(input, "|");
    while (token != NULL && num_cmds < MAX_CMDS) {
        commands[num_cmds++] = token;
        token = strtok(NULL, "|");
    }

    // Loop through each command in the pipeline
    for (int i = 0; i < num_cmds; i++) {
        char **args = split_command(commands[i]);

        // If this is not the last command, create a pipe
        if (i < num_cmds - 1) {
            if (pipe(pipefd) == -1) {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }

        // Fork a new process
        pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            // In the child process

            // If not the first command, redirect input from the previous pipe
            if (input_fd != 0) {
                dup2(input_fd, STDIN_FILENO);
                close(input_fd);
            }

            // If not the last command, redirect output to the current pipe
            if (i < num_cmds - 1) {
                close(pipefd[0]);  // Close unused read end
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
            }

            // Handle redirection (<, >)
            handle_redirection(args);

            // Execute the command
            if (execvp(args[0], args) == -1) {
                perror("execvp");
                exit(EXIT_FAILURE);
            }
        } else {
            // In the parent process

            // Close the input file descriptor from the previous command
            if (input_fd != 0) {
                close(input_fd);
            }

            // Close the write end of the pipe
            if (i < num_cmds - 1) {
                close(pipefd[1]);
                input_fd = pipefd[0];  // Set input for the next command to the read end of the pipe
            }

            // Wait for the child process to finish
            waitpid(pid, NULL, 0);
        }

        // Free dynamically allocated memory
        free(args);
    }
}