#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <time.h>
#include <time.h>
#include <termios.h>
#include "background.h"
pid_t get_most_recent_process();
void set_nonblocking_input();
void reset_input_mode();
void print_recent_process_interval(int interval);


