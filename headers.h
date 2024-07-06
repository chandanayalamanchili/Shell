#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <sys/wait.h>
#include <dirent.h>
#include <sys/stat.h>
#include <grp.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>
#include <sys/time.h> // Include this for gettimeofday
#include <sys/types.h>
#include <signal.h>
#include <dirent.h>
#include <termios.h>
#include <stdbool.h>

#include <netdb.h>
#include <arpa/inet.h>

#define MAX_RESPONSE_SIZE 40000
#define MAX_PATH_LEN 4096
#define MAX_CMD_LEN 256
#define MAX_HISTORY 15
#define MAX_BACKGROUND_PROCESSES 50
// #define MAX_BACKGROUND_PROCESSES 10

// // Declare the shellProcesses array globally
// pid_t shellProcesses[MAX_BACKGROUND_PROCESSES];

// // Global variables to store command history and count
// char past_commands[MAX_HISTORY][MAX_CMD_LEN];
#include"struct.h"

void getHostName(char *host);
void getUserName(char *user);
void getHomeDirectory(char *cwd, const char *home_dir);
void getCurrentDirectory(char *cwd);
int handleWarp(const char *arg, char *curr_dir, char cwd[], char shell_home[]);
int pathExists(const char *path);
void listDirectory(const char *path, int showHidden, int showDetails);
void handlePeek(const char *arg, char cwd[], char shell_home[]);
void add_command(const char *command);
void print_past_commands();
void execute_past_command(int index);
void executeBackgroundCommand(const char *command);
void monitorBackgroundProcesses();
void getProcessInfo(int pid, const char *shell_home);
int redirectInput(const char *input_file);
int redirectOutput(const char *output_file);
int appendOutput(const char *output_file);
int appendOutputFile(const char *output_file);
void executeCommandWithPipes(char *command, char cwd[], char shell_home[]);
void executeCommandWithPipesAndRedirection(char *command, char cwd[], char shell_home[]);
void listActivities();
void sigintHandler(int signo);
void sigtstpHandler(int signo);
void sendSignalToProcess(int pid, int signalNumber);
void pingCommand(const char *arg);
void fgCommand(const char *arg) ;
void bgCommand(const char *arg);
void set_terminal_raw_mode();
void reset_terminal_mode();
void print_most_recent_pid();
void neonateCommand(int time_arg);
void error(const char *msg);
void print_man_page(const char *content);
void fetchAndDisplayManPage(const char *command_name);
void iManCommand(const char *arg);

// int num_commands = 0;

// struct BackgroundProcess
// {
//     pid_t pid;
//     char command[MAX_CMD_LEN];
//     time_t startTime;
//     int status; // 0: Running, 1: Completed normally, 2: Completed abnormally
// };

// struct BackgroundProcess backgroundProcesses[MAX_BACKGROUND_PROCESSES];
// int numBackgroundProcesses = 0;

// pid_t foregroundProcessPid = -1;
// int isForegroundRunning = 0;
