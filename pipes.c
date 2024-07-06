#include"headers.h"

extern int numBackgroundProcesses;

extern pid_t foregroundProcessPid;
extern int isForegroundRunning;
extern int num_commands;
extern struct BackgroundProcess backgroundProcesses[MAX_BACKGROUND_PROCESSES];
extern pid_t shellProcesses[MAX_BACKGROUND_PROCESSES];

extern char past_commands[MAX_HISTORY][MAX_CMD_LEN];

// Function to execute a command with pipes
void executeCommandWithPipes(char *command, char cwd[], char shell_home[])
{
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t child_pid = fork();

    if (child_pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (child_pid == 0)
    {
        
        close(pipe_fd[0]); // Close the read end of the pipe
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);

        // Execute the command using the shell
        char *shell_command = strdup(command);
        system(shell_command);
        free(shell_command);
        exit(EXIT_SUCCESS);
    }
    else
    {
        close(pipe_fd[1]);

        char buffer[MAX_CMD_LEN];
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytes_read;
        while ((bytes_read = read(pipe_fd[0], buffer, sizeof(buffer) - 1)) > 0)
        {
            buffer[bytes_read] = '\0'; 
            printf("%s", buffer);      // Print the output
        }

        close(pipe_fd[0]);

        // Wait for the child process to complete
        wait(NULL);
    }
}
