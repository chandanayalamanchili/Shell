#include"headers.h"

extern int numBackgroundProcesses;

extern pid_t foregroundProcessPid;
extern int isForegroundRunning;
extern int num_commands;
 extern struct BackgroundProcess backgroundProcesses[MAX_BACKGROUND_PROCESSES];

extern pid_t shellProcesses[MAX_BACKGROUND_PROCESSES];

extern char past_commands[MAX_HISTORY][MAX_CMD_LEN];
// Function to list background processes
void listActivities() {
    printf("Activities:\n");

    char proc_dir[MAX_PATH_LEN];
    struct dirent *entry;

    // Open the /proc directory
    DIR *proc = opendir("/proc");
    if (proc == NULL) {
        perror("opendir");
        return;
    }

    // Iterate through the /proc directory
    while ((entry = readdir(proc)) != NULL) {
        // Check if the entry is a directory and its name consists of digits
        if (entry->d_type == DT_DIR && isdigit(entry->d_name[0])) {
            int pid = atoi(entry->d_name);
            char stat_file[MAX_PATH_LEN];
            snprintf(stat_file, sizeof(stat_file), "/proc/%d/stat", pid);

            
            FILE *stat_fp = fopen(stat_file, "r");
            if (stat_fp != NULL) {
                char state;
                fscanf(stat_fp, "%*d %*s %c", &state);
                fclose(stat_fp);

                // Check if the process is one of the background processes
                for (int i = 0; i < numBackgroundProcesses; i++) {
                    if (backgroundProcesses[i].pid == pid) {
                        // Process found in backgroundProcesses array
                        const char* command = backgroundProcesses[i].command;
                        printf("[%d] : %s - %s\n", pid, command, (state == 'T') ? "Stopped" : "Running");
                        break;
                    }
                }
            }
        }
    }

    closedir(proc);
}
