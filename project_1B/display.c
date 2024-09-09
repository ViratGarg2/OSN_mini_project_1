#include"display.h"

void display(char*c,char*g){
    
    char loc[10000];
    char* p = getcwd(loc, sizeof(loc));
        if (p == NULL) {
            perror("getcwd() error");
        }
        if (strcmp(p, c) == 0) {
            
                printf("%s<%s@home:~%s>%s",magenta,g,time_command,reset);

        } else if (strncmp(p,c, strlen(c)) == 0) {
            printf("%s<%s@home:~%s %s>%s",magenta,g,p+strlen(c),time_command,reset);
           
        } else {
  
            printf("%s<%s~%s>%s",blue,p,time_command,reset);
            
        }
}

void hop_display(char*c,char*g){
    char loc[10000];
    char* p = getcwd(loc, sizeof(loc));
        if (p == NULL) {
            perror("getcwd() error");
        }
        if (strcmp(p, c) == 0) {
            printf("/home/%s",g);
        } else if (strncmp(p,c, strlen(c)) == 0) {
           // printf("jo");
            printf("/home/%s%s",g,p+strlen(c));
        } else {
            printf("%s~", p);
        }
}