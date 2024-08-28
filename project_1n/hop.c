#include"hop.h"
#include"display.h"

void hop(char* p,char* command,char *c,char*g){
    char*l = command;
    char* x;
    x = strtok(NULL," ");
   // printf("%s",x);
    // char PREV_DIR[10000];
    if(x == NULL){
        printf("Some location required\n");
        return;
    }
    while(x!=NULL){
        if(x[0] == '~'){
            getcwd(PREV_DIR,sizeof(PREV_DIR));
            int k = chdir(c);
            if(k!=0){
                perror("error");
                break;
            }
            printf("hi%s\n",PREV_DIR);
        }
        else if(x[0] == '-'){
            if(PREV_DIR == NULL){
                printf("No previous cd command found\n");
                return;
            }
            char* temp = (char*)malloc(1000*sizeof(char));
            getcwd(temp,sizeof(temp));
            int k = chdir(PREV_DIR);
            if(k!=0){
                perror("error");
                break;
            }
            strcpy(PREV_DIR,temp);
        }else{
        getcwd(PREV_DIR,sizeof(PREV_DIR));
        int k = chdir(x);
        if(k != 0){
            perror("error");
            break;
        }
        
        }
        x = strtok(NULL," ");
    } 
    hop_display(c,g);
    printf("\n");
}