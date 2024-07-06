#include"headers.h"

extern int numBackgroundProcesses;

extern pid_t foregroundProcessPid;
extern int isForegroundRunning;
extern int num_commands;
extern struct BackgroundProcess backgroundProcesses[MAX_BACKGROUND_PROCESSES];
extern pid_t shellProcesses[MAX_BACKGROUND_PROCESSES];

extern char past_commands[MAX_HISTORY][MAX_CMD_LEN];

int handleWarp(const char *arg, char *curr_dir, char cwd[], char shell_home[])
{
    char new_dir[MAX_PATH_LEN];
    char current_dir[MAX_PATH_LEN];

    // Get the current working directory before changing it
    if (getcwd(current_dir, MAX_PATH_LEN) == NULL)
    {
        perror("getcwd");
        exit(EXIT_FAILURE);
    }
    if (strcmp(arg, "~") == 0)
    {
        strcpy(cwd, "~");
        strcpy(new_dir, shell_home);
        strcpy(curr_dir, shell_home);
        // printf("%s...a\n", curr_dir);
    }

    else if (strcmp(arg, "-") == 0)
    {
        // Handle the "-" flag to go back to the previous directory
        char *prev_dir = getenv("OLDPWD");
        if (prev_dir == NULL)
        {
            printf("No previous directory set.\n");
            return 0; // Indicate failure
        }
        strcpy(cwd, prev_dir + strlen(shell_home));
        strcpy(new_dir, prev_dir);
    }
    else if (arg[0] == '~' && strlen(arg) != 1)
    {
        // Handle the "~" flag to represent the home directory of the shell
        strcpy(new_dir, shell_home);
        strcat(new_dir, "/");     
        strcat(new_dir, arg + 3); 

        // Check if the directory exists before changing the prompt
        if (access(cwd, F_OK) == -1)
        {
            printf("Directory '%s' does not exist.\n", new_dir);
            return 0; // Indicate failure
        }

        strcpy(cwd, arg + 1); 
    }
    else if (strncmp(arg, "..", 2) == 0)
    {
        // Handle going up one directory level
        char *last_slash = strrchr(curr_dir, '/');
        if (last_slash != NULL)
        {
            *last_slash = '\0'; // Remove the last directory
            strcpy(new_dir, curr_dir);
            strcat(new_dir, "/");
            strcat(new_dir, arg + 3); // Append the directory specified after ".."

            // Check if the directory exists before changing the prompt
            if (access(new_dir, F_OK) == -1)
            {
                printf("Directory '%s' does not exist.\n", new_dir);
                return 0; // Indicate failure
            }

            strcpy(cwd, new_dir + strlen(shell_home));
        }
        else
        {
            printf("Cannot go up further; already at the root.\n");
            return 0; // Indicate failure
        }
    }

    else if (arg[0] == '/')
    {
        // Handle absolute paths
        strcpy(new_dir, arg);
        strcpy(cwd, new_dir);
    }
    else
    {
        strcpy(new_dir, curr_dir); // Start with the current directory
        strcat(new_dir, "/");
        strcat(new_dir, arg);

        // Check if the directory exists before changing the prompt
        if (access(new_dir, F_OK) == -1)
        {
            printf("Directory '%s' does not exist.\n", new_dir);
            return 0; // Indicate failure
        }

        strcpy(cwd, new_dir + strlen(shell_home));
    }

    // Change the working directory and update the OLDPWD environment variable
    if (chdir(new_dir) == 0)
    {
        setenv("OLDPWD", current_dir, 1); // Update OLDPWD with the previous working directory
        setenv("PWD", new_dir, 1);
        return 1; // Indicate success
    }
    else
    {
        perror("chdir");
        return 0; // Indicate failure
    }
}

