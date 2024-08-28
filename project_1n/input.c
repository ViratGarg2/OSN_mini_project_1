#include"input.h"
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

void input(char* buffer2,int file,char* p,char* c,char *g,int k,int flg,char* save,char* command2){

        // char interfare[] = {';', '\n'};
        

        //    command2 =  strtok(NULL,";");
        //     printf("%s",command2);
        // break;
        // char *command = strtok(buffer2, " ");

            
            char *command = strtok(command2, " ");

            while (command != NULL)
            {
                // printf("%s\n", command);
                // printf("command is %s\n", command);
                // char trimmed_command[1000]; // Trim leading spaces/tabs
                // strcpy(trimmed_command,command);
                // trimmed_command[strlen(trimmed_command)] = '\n';
                // printf("token is %s",trimmed_command);
                //  strtok(trimmed_command," ");
                // printf("%s",trimmed_command);
                //  trimmed_command = strtok(trimmed_command," ");
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
                    printf("hi");
                    hop(p, command, c, g);
                }
                else if (strcmp(command, "log") == 0)
                {
                    char *next_token = strtok(NULL, " ");
                    if (next_token == NULL)
                    {
                        print_log(file);
                    }
                    else if (strcmp(next_token, "purge") == 0)
                    {
                        remove(filepath);
                    }
                    else if (strcmp(next_token, "execute") == 0)
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
                                int file = open("newfile.txt", O_WRONLY | O_APPEND | O_CREAT, 0644);
                                ssize_t bytes_written = write(file,buffer, strlen(buffer));
                                // strcpy(buffer2, buffer);
                                
                                // fflush(stdout);
                                // int terminal = dup(STDOUT_FILENO);
                                // dup2(file, STDOUT_FILENO);
                                // printf("previous command: %s", buffer);
                                // fflush(stdout);
                                // dup2(terminal, STDOUT_FILENO);
                                // close(file);
                                close(file);
                                input(buffer,file,p,c,g,k,flg,save,command2); // Break to process the next command from buffer2
                            }
                        }
                    }
                }
                else if (strcmp(command, "proclore") == 0)
                {
                    char *next_token = strtok(NULL, " ");

                    if (next_token == NULL)
                    {
                        next_token = malloc(12);
                        if (next_token == NULL)
                        {
                            perror("malloc() error");
                            return;
                        }
                        snprintf(next_token, 12, "%d", k);
                        next_token[11] = '\0';
                    }
                    print_process_info(next_token);
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
                command = strtok(NULL, " ");
            }

            // flg = 0;
            // Move to the next command
            // if (command2 == NULL)
            //     printf("Fuck off");

            // printf("%ssofn",command2);
        
}