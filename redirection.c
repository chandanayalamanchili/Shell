#include"headers.h"

extern int numBackgroundProcesses;

extern pid_t foregroundProcessPid;
extern int isForegroundRunning;
extern int num_commands;
extern struct BackgroundProcess backgroundProcesses[MAX_BACKGROUND_PROCESSES];
extern pid_t shellProcesses[MAX_BACKGROUND_PROCESSES];

extern char past_commands[MAX_HISTORY][MAX_CMD_LEN];

int redirectInput(const char *input_file)
{
    int fd = open(input_file, O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        return -1;
    }
    if (dup2(fd, STDIN_FILENO) == -1)
    {
        perror("dup2");
        return -1;
    }
    close(fd);
    return 0;
}

// Function to redirect output to a file (overwrite)
int redirectOutput(const char *output_file)
{
    int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("open");
        return -1;
    }
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        perror("dup2");
        return -1;
    }
    close(fd);
    return 0;
}

// Function to redirect output to a file (append)
int appendOutput(const char *output_file)
{
    int fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1)
    {
        perror("open");
        return -1;
    }
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        perror("dup2");
        return -1;
    }
    close(fd);
    return 0;
}

// Function to append output to a file (>>)
int appendOutputFile(const char *output_file)
{
    int fd_out = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd_out == -1)
    {
        perror("open");
        return 0;
    }
    dup2(fd_out, STDOUT_FILENO);
    close(fd_out);
    return 1;
}
