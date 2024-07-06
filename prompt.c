#include"headers.h"

extern int numBackgroundProcesses;

extern pid_t foregroundProcessPid;
extern int isForegroundRunning;
extern int num_commands;
 extern struct BackgroundProcess backgroundProcesses[MAX_BACKGROUND_PROCESSES];
 extern pid_t shellProcesses[MAX_BACKGROUND_PROCESSES];

extern char past_commands[MAX_HISTORY][MAX_CMD_LEN];

// Function to get the hostname
void getHostName(char *host)
{
    if (gethostname(host, MAX_PATH_LEN) != 0)
    {
        perror("gethostname");
        exit(EXIT_FAILURE);
    }
}

// Function to get the username
void getUserName(char *user)
{
    if (getlogin_r(user, MAX_PATH_LEN) != 0)
    {
        perror("getlogin_r");
        exit(EXIT_FAILURE);
    }
}

// Function to get the home directory and format it as "~"
void getHomeDirectory(char *cwd, const char *home_dir)
{
    strcpy(cwd, "~");
}

// Function to get the current working directory and format it
void getCurrentDirectory(char *cwd)
{
    if (getcwd(cwd, MAX_PATH_LEN) == NULL)
    {
        perror("getcwd");
        exit(EXIT_FAILURE);
    }
}
