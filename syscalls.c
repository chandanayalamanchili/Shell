#include"headers.h"

extern int numBackgroundProcesses;

extern pid_t foregroundProcessPid;
extern int isForegroundRunning;
extern int num_commands;
extern struct BackgroundProcess backgroundProcesses[MAX_BACKGROUND_PROCESSES];
extern pid_t shellProcesses[MAX_BACKGROUND_PROCESSES];

extern char past_commands[MAX_HISTORY][MAX_CMD_LEN];
// Function to execute a background command
void executeBackgroundCommand(const char *command)
{
    // Create a new process to execute the background command
    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        // Child process: Execute the background command
        execlp("sh", "sh", "-c", command, NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    }
    else
    {
        // Parent process: Store information about the background process
        struct BackgroundProcess newProcess;
        newProcess.pid = pid;
        strncpy(newProcess.command, command, sizeof(newProcess.command) - 1);
        newProcess.startTime = time(NULL);
        newProcess.status = 0; // Running

        // Add the new background process to the array
        backgroundProcesses[numBackgroundProcesses++] = newProcess;

        // Print the PID of the background process
        printf("[%d]\n", pid);
    }
}

// Function to monitor and update background processes
void monitorBackgroundProcesses()
{
    int i;
    for (i = 0; i < numBackgroundProcesses; i++)
    {
        struct BackgroundProcess *process = &backgroundProcesses[i];
        int status;
        pid_t result = waitpid(process->pid, &status, WNOHANG);

        if (result == -1)
        {
            perror("waitpid");
            exit(EXIT_FAILURE);
        }
        else if (result > 0)
        {
            // The background process has completed
            if (WIFEXITED(status))
            {
                process->status = 1; // Completed normally
                printf("Sleep exited normally (%d)\n", process->pid);
            }
            else
            {
                process->status = 2; // Completed abnormally
                printf("Sleep exited abnormally (%d)\n", process->pid);
            }

            // Print a message with process details
            printf("[%d] : %s\n", process->pid, process->command);

            // Remove the completed process from the array
            if (i < numBackgroundProcesses - 1)
            {
                backgroundProcesses[i] = backgroundProcesses[numBackgroundProcesses - 1];
            }
            numBackgroundProcesses--;
            i--;
        }
    }
}
