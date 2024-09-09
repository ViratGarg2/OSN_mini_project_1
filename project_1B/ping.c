#include "ping.h"
void send_signal_to_pid(int pid, int signal) {
    // Check if the PID exists and is valid
    if (kill(pid, 0) == -1) {
        perror("Error: Invalid PID or process does not exist");
        return;
    }

    // Send the specified signal to the process with the given PID
    if (kill(pid, signal) == 0) {
        printf("sent signal %d successfully to process with pid %d\n", signal, pid);
    } else {
        perror("Error sending signal");
    }
}