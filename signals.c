#include"headers.h"

extern int numBackgroundProcesses;

extern pid_t foregroundProcessPid;
extern int isForegroundRunning;
extern int num_commands;
extern struct BackgroundProcess backgroundProcesses[MAX_BACKGROUND_PROCESSES];
extern pid_t shellProcesses[MAX_BACKGROUND_PROCESSES];

extern char past_commands[MAX_HISTORY][MAX_CMD_LEN];
// Signal handler for SIGINT (Ctrl-C)
void sigintHandler(int signo)
{
    if (foregroundProcessPid != -1)
    {
        kill(foregroundProcessPid, SIGINT);
    }
}

void sigtstpHandler(int signo)
{
    if (foregroundProcessPid != -1)
    {
        // Send SIGTSTP to the foreground process
        kill(foregroundProcessPid, SIGTSTP);
        isForegroundRunning = 0; // Update foreground process status
    }
}

// Function to send a signal to a process and handle errors
void sendSignalToProcess(int pid, int signalNumber)
{
    if (kill(pid, signalNumber % 32) == 0)
    {
        printf("Sent signal %d to process with pid %d\n", signalNumber % 32, pid);
    }
    else
    {
        perror("kill");
        printf("No such process found with pid %d\n", pid);
    }
}
// Function to send a signal to a process and handle errors
void pingCommand(const char *arg) {
    // Parse the arguments
    int pid;
    int signal_number;
    if (sscanf(arg, "%d %d", &pid, &signal_number) != 2) {
        printf("Invalid syntax for ping command. Usage: ping <pid> <signal_number>\n");
        return;
    }

    // Send the signal to the specified process
    int result = kill(pid, signal_number % 32);

    if (result == 0) {
        printf("Sent signal %d to process with pid %d\n", signal_number % 32, pid);
        
       
    } else {
        printf("Error: No such process found with pid %d\n", pid);
    }
}
