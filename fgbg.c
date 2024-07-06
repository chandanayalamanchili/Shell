#include"headers.h"

extern int numBackgroundProcesses;

extern pid_t foregroundProcessPid;
extern int isForegroundRunning;
extern int num_commands;
 extern struct BackgroundProcess backgroundProcesses[MAX_BACKGROUND_PROCESSES];
extern pid_t shellProcesses[MAX_BACKGROUND_PROCESSES];

extern char past_commands[MAX_HISTORY][MAX_CMD_LEN];
void fgCommand(const char *arg) {
    int pid;
    if (sscanf(arg, "%d", &pid) != 1) {
        printf("Invalid syntax for fg command. Usage: fg <pid>\n");
        return;
    }

    int found = 0;

            found = 1;
            tcsetpgrp(STDIN_FILENO, pid); // Set the process group
            signal(SIGTTIN, SIG_IGN);
            signal(SIGTTOU, SIG_IGN);
            int status;

            kill(pid, SIGCONT);
            waitpid(-1, &status, WUNTRACED);

            
            tcsetpgrp(STDIN_FILENO, getpgrp()); // Set the shell process group
            signal(SIGTTIN, SIG_DFL);
            signal(SIGTTOU, SIG_DFL);
     
}

// Function to change the state of a stopped background process to running
void bgCommand(const char *arg) {
    int pid;
    if (sscanf(arg, "%d", &pid) != 1) {
        printf("Invalid syntax for bg command. Usage: bg <pid>\n");
        return;
    }

    int found = 0;

    for (int i = 0; i < numBackgroundProcesses; i++) {
        if (backgroundProcesses[i].pid == pid) {
            found = 1;
            // Check the status of the process in /proc/pid/stat
            char statFilePath[256];
            snprintf(statFilePath, sizeof(statFilePath), "/proc/%d/stat", pid);

            FILE *statFile = fopen(statFilePath, "r");
            if (statFile) {
                char statData[1024];
                if (fgets(statData, sizeof(statData), statFile) != NULL) {
                    char *statPtr = strtok(statData, " ");
                    for (int j = 0; j < 2; j++) {
                        statPtr = strtok(NULL, " ");
                    }
                    if (statPtr && strcmp(statPtr, "T") == 0) {
                        // Process is in stopped state (T)
                        backgroundProcesses[i].status = 0; // Running
                        kill(pid, SIGCONT); // Send SIGCONT to resume the process
                    } else {
                        printf("Process with pid %d is not in a stopped state.\n", pid);
                    }
                }
                fclose(statFile);
            } else {
                printf("Error opening stat file for process with pid %d\n", pid);
            }
            break;
        }
    }

    if (!found) {
        printf("No such process found with pid %d\n", pid);
    }
}
