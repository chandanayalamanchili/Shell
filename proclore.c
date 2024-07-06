#include"headers.h"

extern int numBackgroundProcesses;

extern pid_t foregroundProcessPid;
extern int isForegroundRunning;
extern int num_commands;
extern struct BackgroundProcess backgroundProcesses[MAX_BACKGROUND_PROCESSES];
extern pid_t shellProcesses[MAX_BACKGROUND_PROCESSES];

extern char past_commands[MAX_HISTORY][MAX_CMD_LEN];

// Function to get process information
void getProcessInfo(int pid, const char *shell_home)
{
    char proc_path[MAX_PATH_LEN];
    char status_path[MAX_PATH_LEN];
    char exe_path[MAX_PATH_LEN];

    snprintf(proc_path, sizeof(proc_path), "/proc/%d", pid);

    if (access(proc_path, F_OK) == -1)
    {
        printf("Process with PID %d does not exist.\n", pid);
        return;
    }

    snprintf(status_path, sizeof(status_path), "/proc/%d/status", pid);

    FILE *status_file = fopen(status_path, "r");
    if (status_file == NULL)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    char line[MAX_PATH_LEN];
    char process_status[MAX_PATH_LEN];
    int process_group = 0;
    unsigned long vm_size = 0;

    while (fgets(line, sizeof(line), status_file))
    {
        if (sscanf(line, "State: %s", process_status) == 1)
        {
            // Process Status
            if (process_status[0] == 'R' || process_status[0] == 'S' || process_status[0] == 'Z')
            {
                if (line[1] == '+')
                {
                    strcat(process_status, "+");
                }
            }
        }
        else if (sscanf(line, "VmSize: %lu kB", &vm_size) == 1)
        {
            vm_size /= 1024;
        }
        else if (sscanf(line, "Tgid: %d", &process_group) == 1)
        {
            
        }
    }

    fclose(status_file);

    snprintf(exe_path, sizeof(exe_path), "/proc/%d/exe", pid);

    char absolute_exe_path[MAX_PATH_LEN];
    ssize_t exe_length = readlink(exe_path, absolute_exe_path, sizeof(absolute_exe_path) - 1);
    if (exe_length != -1)
    {
        absolute_exe_path[exe_length] = '\0';

        char relative_exe_path[MAX_PATH_LEN];
        if (strncmp(absolute_exe_path, shell_home, strlen(shell_home)) == 0)
        {
            snprintf(relative_exe_path, sizeof(relative_exe_path), "~%s", absolute_exe_path + strlen(shell_home));
        }
        else
        {
            strcpy(relative_exe_path, absolute_exe_path);
        }

        // Print the process information
        printf("pid: %d\n", pid);
        printf("process status: %s\n", process_status);
        printf("Process Group: %d\n", process_group);
        printf("Virtual memory: %lu MB\n", vm_size);
        printf("Executable path: %s\n", relative_exe_path);
    }
    else
    {
        perror("readlink");
        exit(EXIT_FAILURE);
    }
}
