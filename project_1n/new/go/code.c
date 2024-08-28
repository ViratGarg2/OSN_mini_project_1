#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "display.h"
#include "reveal.h"
#include "hop.h"
#include "log.h"
#include "input.h"

char PREV_DIR[10000];
int cnt = 0;
char *trim(char *str, const char *unwantedChars)
{
    while (*str && strchr(unwantedChars, *str))
    {
        str++;
    }
    if (*str == '\0')
    {
        return str;
    }
    char *end = str + strlen(str) - 1;
    while (end > str && strchr(unwantedChars, *end))
    {
        end--;
    }
    *(end + 1) = '\0';
    return str;
}
char *filepath = "/home/viratgarg/Documents/Sem3/OSN/projects/project_1n/newfile.txt";

int main()
{
    char buffer[1024];
    char *c = getcwd(buffer, sizeof(buffer));
    char *g = getenv("LOGNAME");
    if (c == NULL)
    {
        perror("getcwd() error");
        return 1;
    }
    char *store;
    char buffer3[1000];
    int flg = 0;
    char buffer2[1024];
    while (1)
    {
        if (flg == 0)
        {
            display(c, g);
        }
        char loc[1000];
        char *p = getcwd(loc, sizeof(loc));
        if (flg == 0)
        {
            if (fgets(buffer2, sizeof(buffer2), stdin) == NULL)
            {
                perror("fgets() error");
                continue;
            }
        }
        char *prev;
        // prev = extract_last_line("newfile.txt",buffer3);
        // printf("previous command is %s",prev);

        char x[] = "log\n";
        int file = open(filepath, O_RDWR | O_CREAT | O_APPEND, 0644);

        // printf("some characters are %c %c",buffer2[3],x[2]);
        if (strcmp(buffer3, buffer2) == 0)
        {
            // same as previous command

            close(file);
            printf("same as previous command\n");
        }
        else if (strcmp(buffer2, x) == 0)
        {
            // log is entered
            close(file);
        }
        else
        {
            file = open(filepath, O_RDWR | O_CREAT | O_APPEND, 0644);
            int terminal = dup(STDOUT_FILENO);
            dup2(file, STDOUT_FILENO);
            printf("%s", buffer2);
            fflush(stdout);
            dup2(terminal, STDOUT_FILENO);
            close(file);
        }
        strcpy(buffer3, buffer2);
        buffer2[strcspn(buffer2, "\n")] = '\0';
        char interfare[] = {';', '\n'};
        char *command = strtok(buffer2, ";");
        while (command != NULL)
        {
            printf("command is %s\n", command);
            char * trimmed_command = strtok(command, " \t"); // Trim leading spaces/tabs

            if (command == NULL)
            {
                printf("No command entered\n");
                continue;
            }

            if (strcmp(command, "cd") == 0)
            {
                char *path = strtok(NULL, " ");
                if (path != NULL)
                {
                    if (chdir(path) != 0)
                    {
                        perror("chdir() error");
                    }
                }
                else
                {
                    printf("Path not specified for cd command\n");
                }
            }
            else if (strcmp(command, "reveal") == 0)
            {
                reveal(p, command);
            }
            else if (strcmp(command, "hop") == 0)
            {
                hop(p, command, c, g);
            }
            else if (strcmp(command, "log") == 0)
            {
                command = strtok(NULL, " ");
                if (command == NULL)
                {
                    print_log(file);
                }
                else if (strcmp(command, "purge") == 0)
                {
                    remove("newfile.txt");
                }
                else if (strcmp(command, "execute") == 0)
                {
                    char *line = strtok(NULL, " ");
                    if (line == NULL)
                    {
                        printf("No line number specified\n");
                    }
                    else
                    {
                        int cnt2 = count_lines_in_file(filepath);
                        int line_number = atoi(line);
                        if (cnt2 < line_number)
                        {
                            printf("Line number out of range\n");
                        }
                        else
                        {
                            char buffer_execute[1024];
                            char *buffer = execute_log(line_number, buffer_execute);
                            strcat(buffer, "\n");
                            strcpy(buffer2, buffer);
                            flg = 1;
                            break; // Break to process the next command from buffer2
                        }
                    }
                }
            }
            else if (strcmp(command, "echo") == 0)
            {
                char *message = strtok(NULL, "");
                if (message != NULL)
                {
                    message = trim(message, "; \t");
                    printf("%s\n", message);
                }
                else
                {
                    printf("\n");
                }
            }
            else if (strcmp(command, "ls") == 0)
            {
                char *path = strtok(NULL, " ");
                char cmd[1024] = "ls";
                while (path != NULL)
                {
                    strcat(cmd, " ");
                    strcat(cmd, path);
                    path = strtok(NULL, " ");
                }
                int result = system(cmd);
                if (result == -1)
                {
                    perror("system() error");
                }
            }

            flg = 0;
            //printf("command is %s", command);
            command = strtok(NULL, ";"); // Move to the next command
        }
    }
}
