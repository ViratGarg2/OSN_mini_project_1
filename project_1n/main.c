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

char PREV_DIR[100000];
int cnt = 0;

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
    while (1)
    {   
        
        if (flg == 0)
        {
            display(c, g);
        }
        char loc[1000];
        char *p = getcwd(loc, sizeof(loc));
       //  printf("gr8");
        if (flg == 0)
        {
            if (fgets(buffer2, sizeof(buffer2), stdin) == NULL)
            {
                perror("fgets() error");
                continue;
            }
        }
      //  fflush(stdin);
       
        int file;
        char buffer3[10000];
        extract_last_line(filepath,buffer3,10000);
        
      // printf("buffer is %s",buffer3);
      fflush(stdout);
     //   printf("Last line:%s",buffer3);
   // char *prev = extract_last_line(filepath,buffer3);
//    if(buffer3 == NULL){
//        printf("buffer3 is null");
//    }
   // printf("buffer3->%s",buffer3);
   // printf("iubv");
     char x[] = "log\n";
     file = open(filepath, O_RDWR | O_CREAT | O_APPEND, 0644);
    // printf("lp");
    if (strcmp(buffer3, buffer2) == 0)
        {
            // same as previous command
            close(file);
            printf("same as previous command\n");
            continue;
        }
        else if (strncmp(buffer2, x,3) == 0)
        {
            // log is entered
            close(file);
        }
        else
        {
            file = open(filepath, O_RDWR | O_CREAT | O_APPEND, 0644);
            ssize_t bytes_written = write(file,buffer2, strlen(buffer2));
        }
        strcpy(buffer3, buffer2);
        buffer2[strcspn(buffer2, "\n")] = '\0';
        char *save;
        char *command2 = strtok_r(buffer2, ";", &save);
        while(command2!=NULL){
        input(buffer2,file,p,c,g,k,flg,save,command2);
        command2 = strtok_r(NULL, ";", &save);
        }
        // printf("some characters are %c %c",buffer2[3],x[2]);
       
    }
}
