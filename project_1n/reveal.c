#include"reveal.h"
#define PATH_MAX 4096

void print_file_permissions(mode_t mode) {
    char permissions[11];
    
    //these permissions tell about the file type
    permissions[0] = (S_ISDIR(mode)) ? 'd' : '-';
    permissions[0] = (S_ISCHR(mode)) ? 'c' : permissions[0];
    permissions[0] = (S_ISBLK(mode)) ? 'b' : permissions[0];
    permissions[0] = (S_ISFIFO(mode)) ? 'p' : permissions[0];
    permissions[0] = (S_ISLNK(mode)) ? 'l' : permissions[0];
    permissions[0] = (S_ISSOCK(mode)) ? 's' : permissions[0];

//these permissions tell us about read,write and execute for owners as well as groups
    permissions[1] = (mode & S_IRUSR) ? 'r' : '-';
    permissions[2] = (mode & S_IWUSR) ? 'w' : '-';
    permissions[3] = (mode & S_IXUSR) ? 'x' : '-';
    permissions[4] = (mode & S_IRGRP) ? 'r' : '-';
    permissions[5] = (mode & S_IWGRP) ? 'w' : '-';
    permissions[6] = (mode & S_IXGRP) ? 'x' : '-';
    permissions[7] = (mode & S_IROTH) ? 'r' : '-';
    permissions[8] = (mode & S_IWOTH) ? 'w' : '-';
    permissions[9] = (mode & S_IXOTH) ? 'x' : '-';
    permissions[10] = '\0';

    printf("%s", permissions);
}

// Function to print file information similar to ls -l
void print_file_info(const char *dir_name, const char *file_name) {
    char full_path[PATH_MAX];
    struct stat file_stat;

    // Create full path to the file
    snprintf(full_path, sizeof(full_path), "%s/%s", dir_name, file_name);

    // Get file statistics
    if (stat(full_path,&file_stat) == -1) {
        perror("stat");
        return;
    }

    // Print file permissions
    print_file_permissions(file_stat.st_mode);

    // Print number of hard links
    printf(" %ld", (long)file_stat.st_nlink);

    // Print owner user name
    struct passwd *pw = getpwuid(file_stat.st_uid);
    if (pw != NULL) {
        printf(" %s", pw->pw_name);
    } else {
        printf(" %d", file_stat.st_uid);
    }
    struct group *gr = getgrgid(file_stat.st_gid);
    if (gr != NULL) {
        printf(" %s", gr->gr_name);
    } else {
        printf(" %d", file_stat.st_gid);
    }
    printf(" %lld", (long long)file_stat.st_size);
    char time_buf[80];
    struct tm *time_info;
    time_info = localtime(&file_stat.st_mtime);
    strftime(time_buf, sizeof(time_buf), "%b %d %H:%M", time_info);
    printf(" %s", time_buf);
    printf(" %s\n", file_name);
}
void print_detail(char *str, int flg) {
    DIR *dir = opendir(str);
    if (dir == NULL) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    int i = 0;
    while ((entry = readdir(dir)) != NULL) {
        i++;
        if (flg == 0 && entry->d_name[0] == '.') {
            continue;
        }
        print_file_info(str, entry->d_name);
    }

    closedir(dir);
}
void print_less(char* str,int flg){
    if(str == NULL){
        printf("string is NULL");
        return;
    }
        DIR *dir = opendir(str);
        if (dir == NULL) {
        printf("invalid path\n");
        return;
    }
    struct dirent *entry;
    int i = 0;
    while ((entry = readdir(dir)) != NULL) {
     i++;
    if(flg == 0 && entry->d_name[0] == '.'){
            continue;
    }
     printf("\033[32m%d\033[32m  ",i);

     printf("\033[0m%s\033[0m\n",entry->d_name);
    }
}
void get_parent_directory(const char *path, char *parent_dir) {
    // Copy the input path to a modifiable string
    char temp_path[1024];
    strncpy(temp_path, path, sizeof(temp_path) - 1);
    temp_path[sizeof(temp_path) - 1] = '\0';

    // Find the last occurrence of the directory separator '/'
    char *last_slash = strrchr(temp_path, '/');
    
    if (last_slash != NULL) {
        // If a slash is found, terminate the string at that point
        *last_slash = '\0';
    } else {
        // If no slash is found, the input is either the root directory or an invalid path
        strcpy(temp_path, ".");
    }

    // Copy the result to the output variable
    strncpy(parent_dir, temp_path, 1024);
}

void reveal(char* p,char* command){
    char*l = strtok(NULL," ");
    if(l == NULL){
        print_less(p,0);
        return;
    }
    int flg = 0;
    int acnt = 0;
    int lcnt = 0;
    if(l[0] == '-'){
        for(int i = 0;i<strlen(l);i++){
            if(l[i] == 'a')acnt++;
            if(l[i] == 'l')lcnt++;
            if(acnt>=1){
                flg = 1;
            }
            if(lcnt>=1 && acnt>=1){
                break;
            }
        }
        char*k = strtok(NULL," ");
        if(k!=NULL){
            if(k[0] == '-'){
                printf("%s\n",k);
                for(int i = 0;i<strlen(k);i++){
                    if(k[i] == 'a')acnt++;
                    if(k[i] == 'l')lcnt++;
                    if(acnt>=1){
                        flg = 1;
                    }
                    if(acnt>=1 && lcnt>=1){
                        break;
                    }
                }
                k = strtok(NULL," ");
                if(k == NULL && lcnt>=1){
                    print_detail(p,flg);
                    return;
                }else if(k == NULL && (lcnt<1 || acnt<1)){
                    print_less(p,flg);
                    return;
                }
            }
            if(lcnt>=1){
                print_detail(k,flg);
            }
            else{
                print_less(k,flg);
            }
        }else{
            if(lcnt>=1){
                print_detail(p,flg);
            }
            else{
                print_less(p,flg);
            }
        }
    }
    else if(strlen(l) == 1 && l[0] == '.'){
        print_less(p,0);
        return;
    }
    else if(strcmp(l,"..") == 0){
        char parent[3000];
        get_parent_directory(p,parent);
        print_less(parent,0);
        return;
    }
        else{
            // printf("hi");
            printf("%s",l);
            if(lcnt>=1){
                print_detail(l,flg);
            }
            else{
                print_less(l,flg);
            }
        }   
}