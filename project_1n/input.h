#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "display.h"
#include "reveal.h"
#include "hop.h"
#include "log.h"
#include "proclore.h"

void input(char* buffer2,int file,char*p,char* c,char *g,int k,int flg,char* save,char *command2);
char *trim(char *str, const char *unwantedChars);