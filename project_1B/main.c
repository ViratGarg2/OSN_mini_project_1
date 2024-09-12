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
#include "proclore.h"
#include "find.h"
#include "alias.h"
#include <sys/wait.h>
#include "color.h"
#include<ctype.h>
#include "signals.h"
#include "background.h"
char PREV_DIR[4096];
int foreground_id = -1;
int cnt = 0;
int sleepflg = 0;
int sleepval = 0;
char* slow_commands[100];
char time_command[4096];
char time_command[4096] = "";
char foreground_name2[4096];
char *filepath = "/home/viratgarg/Documents/Sem3/OSN/projects/project_1B/newfile.txt";
char *pids = "/home/viratgarg/Documents/Sem3/OSN/projects/project_1B/pids.txt";
int time2 = 0;
int main()
{
    int k = getpid();
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
    signal(SIGINT, handle_sigint);
    signal(SIGTSTP,ctrl_z);

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
             //   perror("fgets() error");
               
                ctrl_d();
                printf("interrupted\n");
                // exit(0);
            }
        }
        // if(fgets(buffer2, sizeof(buffer2), stdin) == NULL)
        // {
        //     perror("fgets() error");
        //     return 0;
        // }
       // check_background_processes();
        trim(buffer2);
        int file;
        char buffer3[10000];
        extract_last_line(filepath, buffer3, 10000);
        fflush(stdout);
      //  trim(buffer3);
        // buffer2[strlen(buffer2)] = '\n';
        char x[] = "log\n";
file = open(filepath, O_RDWR | O_CREAT | O_APPEND, 0644);

if (file < 0) {
    perror("Error opening file");
    return 0;
}
// printf("buffer2 is %s and buffer3 is %s", buffer2,buffer3);
if (strcmp(buffer3, buffer2) == 0)
{
    // printf("same as previous command\n");
   // buffer2[strlen(buffer2)] = '\n';
    // continue;
}
else if (strncmp(buffer2, x,3) == 0)
{   
    // This case handles the "log\n" scenario
   // buffer2[strlen(buffer2)] = '\n';
    // No need to close here
}
else
{   
  //  printf("buffer2 is %s", buffer2);
    // buffer2[strlen(buffer2)] = '\n';
    strcat(buffer2,"\n");
    ssize_t bytes_written = write(file, buffer2, strlen(buffer2));
    
   // printf("bytes written is %ld\n", bytes_written);
    if (bytes_written < 0) {
        perror("Error writing to file");
    }
}
// printf("%s",buffer2);

input2(buffer2, buffer3, file, p, c, g, k, flg);
close(file);
    }
    
}

