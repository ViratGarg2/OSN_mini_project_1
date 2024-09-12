#include "neonate.h"

pid_t get_most_recent_process() {
    DIR *proc_dir = opendir("/proc");
    struct dirent *entry;
    struct stat statbuf;
    pid_t recent_pid = -1;
    time_t recent_time = 0;

    if (!proc_dir) {
        perror("opendir failed");
        return -1;
    }

    while ((entry = readdir(proc_dir)) != NULL) {
        if (entry->d_name[0] >= '0' && entry->d_name[0] <= '9') {  // Check if it's a PID directory
            char proc_path[500];
            snprintf(proc_path, sizeof(proc_path), "/proc/%s", entry->d_name);

            // Check if it's a directory and get its creation time
            if (stat(proc_path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode)) {
                if (statbuf.st_ctime > recent_time) {
                    recent_time = statbuf.st_ctime;
                    recent_pid = atoi(entry->d_name);  // Update most recent PID
                }
            }
        }
    }
    closedir(proc_dir);
    return recent_pid;
}

// Function to set terminal input to non-blocking mode
void set_nonblocking_input() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);  // Get terminal attributes
    term.c_lflag &= ~(ICANON | ECHO);  // Disable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &term);  // Apply changes

    // Set stdin to non-blocking mode
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
}

// Function to reset terminal input mode
void reset_input_mode() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);  // Get terminal attributes
    term.c_lflag |= (ICANON | ECHO);  // Enable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &term);  // Apply changes
}

// Function to print the most recent process every t seconds, until 'x' is pressed
void print_recent_process_interval(int interval) {
    set_nonblocking_input(); 
    signal(SIGINT,handle_sigint);
     // Set terminal to non-blocking mode
    while (1) {
        // Get the most recent process
        pid_t recent_pid = get_most_recent_process();

        if (recent_pid != -1) {
            printf("Most recent process PID: %d\n", recent_pid);
        } else {
            printf("Could not find a recent process.\n");
        }

        // Sleep for the given interval
        sleep(interval);

        // Check if 'x' key is pressed
        char ch;
        ssize_t nread = read(STDIN_FILENO, &ch, 1);

        // Handle Ctrl+D (EOF) - `nread == 0` means EOF
        if (nread == 0) {
            printf("Detected Ctrl+D, exiting...\n");
            break;
        }

        // Check if 'x' key is pressed
        if (nread > 0 && ch == 'x') {
            printf("'x' pressed. Exiting...\n");
            break;
        }
    }

    reset_input_mode();  // Restore terminal to its normal state
}