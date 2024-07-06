#include"headers.h"
extern int numBackgroundProcesses;

extern pid_t foregroundProcessPid;
extern int isForegroundRunning;
extern int num_commands;
 extern struct BackgroundProcess backgroundProcesses[MAX_BACKGROUND_PROCESSES];
extern pid_t shellProcesses[MAX_BACKGROUND_PROCESSES];

extern char past_commands[MAX_HISTORY][MAX_CMD_LEN];

// Function to set the terminal in raw mode
void set_terminal_raw_mode() {
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

// Function to restore the terminal to its original mode
void reset_terminal_mode() {
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag |= ICANON | ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

// Function to print the most recently created PID
void print_most_recent_pid() {
    FILE *loadavg_file = fopen("/proc/loadavg", "r");
    if (loadavg_file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    char buffer[256];
    if (fgets(buffer, sizeof(buffer), loadavg_file) != NULL) {
        char *last_token = strrchr(buffer, ' ');
        if (last_token != NULL) {
            printf("%s", last_token + 1);
        }
    }

    fclose(loadavg_file);
}

void neonateCommand(int time_arg) {
    // Handle neonate command with the given time_arg
    int pid;
    FILE *loadavgFile;
    char loadavgPath[] = "/proc/loadavg";
    char buffer[128];
    char *token;
    
    while (1) {
        // Open /proc/loadavg file to get the most recently created pid
        loadavgFile = fopen(loadavgPath, "r");
        if (loadavgFile == NULL) {
            perror("fopen");
            return;
        }
        
        // Read the file and parse the last token (most recently created pid)
        fgets(buffer, sizeof(buffer), loadavgFile);
        fclose(loadavgFile);
        
        // Extract the last token (pid)
        token = strtok(buffer, " ");
        while (token != NULL) {
            pid = atoi(token);
            token = strtok(NULL, " ");
        }
        
        // Print the pid
        printf("%d\n", pid);
        
        // Sleep for the specified time_arg in seconds
        sleep(time_arg);
        
        // Check for user input (pressing 'x' to exit)
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            if (buffer[0] == 'x') {
                break;
            }
        }
    }
}