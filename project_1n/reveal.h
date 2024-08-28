#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include<sys/stat.h>
#include<sys/types.h>
#include <time.h>
#include <limits.h>
#include<inttypes.h>
#include<pwd.h>
#include<grp.h>

void get_parent_directory(const char* path,char* parent_dir);
void print_less(char*str,int flg);
void print_detail(char* str,int flg);
void reveal(char*p,char* command);
