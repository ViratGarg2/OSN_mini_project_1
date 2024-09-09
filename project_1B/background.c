#include "background.h"

void check_status(){
    FILE *pid_file = fopen(pids, "r");
    if (pid_file == NULL) {
        printf("No background processes found.\n");
        return;
    }

    char pid[32];
    while (fgets(pid, sizeof(pid), pid_file) != NULL) {
        // Remove newline character if present
        pid[strcspn(pid, "\n")] = 0;
        char* pid2 = strtok(pid, " ");
        char* name = strtok(NULL, "\n");

        char path[256];
        snprintf(path, sizeof(path), "/proc/%s/stat", pid2);
      //  printf("Path is %s\n",path);
        FILE *stat_file = fopen(path, "r");
        if (stat_file == NULL) {
            printf("%s:%s- Stopped.\n", pid,name);
            continue;
        }

        // The third field in the /proc/<PID>/stat file contains the process state
        char comm[256], state;
        int ppid;
        fscanf(stat_file, "%d %s %c", &ppid, comm, &state);
        fclose(stat_file);

        // Determine process state
        const char *status;
        switch (state) {
            case 'R': status = "Running"; break;
            case 'S': status = "Sleeping"; break;
            case 'D': status = "Waiting (Uninterruptible)"; break;
            case 'Z': status = "Zombie (Terminated)"; break;
            case 'T': status = "Stopped"; break;
            default:  status = "Unknown"; break;
        }

        // Print the process status
        printf("%s : %s %s\n", pid,name,status);
    }

    fclose(pid_file);
}