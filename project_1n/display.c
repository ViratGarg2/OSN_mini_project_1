#include"display.h"

void display(char*c,char*g){
    char loc[10000];
    char* p = getcwd(loc, sizeof(loc));
        if (p == NULL) {
            perror("getcwd() error");
        }
        if (strcmp(p, c) == 0) {
            printf("<%s@home:~>",g);
        } else if (strncmp(p,c, strlen(c)) == 0) {
           // printf("jo");
            printf("<%s@home:~%s>",g,p+strlen(c));
        } else {
            printf("%s~", p);
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