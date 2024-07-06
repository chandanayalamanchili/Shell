#include"headers.h"

extern int numBackgroundProcesses;

extern pid_t foregroundProcessPid;
extern int isForegroundRunning;
extern int num_commands;
extern struct BackgroundProcess backgroundProcesses[MAX_BACKGROUND_PROCESSES];
extern pid_t shellProcesses[MAX_BACKGROUND_PROCESSES];

extern char past_commands[MAX_HISTORY][MAX_CMD_LEN];

// Function to add a command to the history if it's not a duplicate
void add_command(const char *command)
{
    // Check if the command is already in the history
    for (int i = 0; i < num_commands; i++)
    {
        if (strcmp(past_commands[i], command) == 0)
        {
            return; 
        }
    }

    if (num_commands < MAX_HISTORY)
    {
        strcpy(past_commands[num_commands], command);
        num_commands++;
    }
    else
    {
        // If the history is full, shift commands to make space for the new one
        for (int i = 0; i < MAX_HISTORY - 1; i++)
        {
            strcpy(past_commands[i], past_commands[i + 1]);
        }
        strcpy(past_commands[MAX_HISTORY - 1], command);
    }
}

// Function to print the command history
void print_past_commands()
{
    for (int i = 0; i < num_commands; i++)
    {
        printf("%s\n", past_commands[i]);
    }
}

// Function to execute a past command by index
void execute_past_command(int index)
{
    if (index < 1 || index > num_commands)
    {
        printf("Invalid index\n");
        return;
    }
    system(past_commands[num_commands - index]);
}
