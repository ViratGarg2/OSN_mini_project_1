       while(command2!=NULL){
        char * command3 = strtok_r(command2,"&",&save2);
        for(int i = 0;i<strlen(command2);i++){
            if(command2[i]=='&'){
                cnt2++;
            }
        }
        printf("%d",cnt2);
        if(command3==NULL){
            input(buffer2,file,p,c,g,k,flg,save,command2);
        }else{
            int background = fork();
            if(background < 0){
                printf("fork failed");
            }else if(background  == 0){
                while(command3!=NULL && cnt2>1){
                    printf("HI");
                    command3 = strtok_r(NULL,"&",&save2);
                }
            }else{
                wait(NULL);
                printf("real_execution");
            }
        // while(command3!=NULL){
            
        //     input(buffer2,file,p,c,g,k,flg,save,command3);
        //     command3 = strtok_r(NULL,"&",&save2);
        // }
        }
        command2 = strtok_r(NULL, ";", &save);
        }