#include"headers.h"

extern int numBackgroundProcesses;

extern pid_t foregroundProcessPid;
extern int isForegroundRunning;
extern int num_commands;
 extern struct BackgroundProcess backgroundProcesses[MAX_BACKGROUND_PROCESSES];
extern pid_t shellProcesses[MAX_BACKGROUND_PROCESSES];

extern char past_commands[MAX_HISTORY][MAX_CMD_LEN];
// Function to execute a command with pipes and I/O redirection
void executeCommandWithPipesAndRedirection(char *command, char cwd[], char shell_home[])
{
    // Create an array to store individual commands after splitting by pipes
    char *commands[32]; // Assuming a maximum of 32 commands
    int num_commands = 0;

    // Split the command into individual commands using '|' as the delimiter
    char *token = strtok(command, "|");
    while (token != NULL)
    {
        commands[num_commands++] = token;
        token = strtok(NULL, "|");
    }

    // Create pipes to connect the commands
    int pipefds[2];
    int inputfd = 0; // File descriptor to read from the previous command's output

    for (int i = 0; i < num_commands; i++)
    {
        pipe(pipefds);

        pid_t child_pid = fork();

        if (child_pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (child_pid == 0)
        {
            // Child process

            // Redirect input from the previous command's output
            dup2(inputfd, 0);
            close(inputfd);

            // If it's not the last command, redirect output to the pipe
            if (i < num_commands - 1)
            {
                dup2(pipefds[1], 1);
            }
            close(pipefds[0]);

            // Execute the command using the shell
            if (execlp("sh", "sh", "-c", commands[i], (char *)NULL) == -1)
            {
                perror("execlp");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            // Parent process

            // Close the write end of the pipe, as it's not needed in the parent
            close(pipefds[1]);

            // Save the read end of the pipe as the input for the next command
            inputfd = pipefds[0];

            // If it's the last command, wait for it to complete
            if (i == num_commands - 1)
            {
                int status;
                waitpid(child_pid, &status, 0);
            }
        }
    }
}