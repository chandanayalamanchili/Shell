#include"headers.h"


struct BackgroundProcess backgroundProcesses[MAX_BACKGROUND_PROCESSES];

int numBackgroundProcesses = 0;

pid_t foregroundProcessPid = -1;
int isForegroundRunning = 0;
int num_commands = 0;

pid_t shellProcesses[MAX_BACKGROUND_PROCESSES];
char past_commands[MAX_HISTORY][MAX_CMD_LEN];


int main()
{
    char host[MAX_PATH_LEN];
    char user[MAX_PATH_LEN];

    getHostName(host);
    getUserName(user);
    struct passwd *pw = getpwuid(getuid());
    const char *home_dir = pw->pw_dir;

    char cwd[MAX_PATH_LEN];
    getHomeDirectory(cwd, home_dir); 
    char shell_home[MAX_PATH_LEN];
    getcwd(shell_home, MAX_PATH_LEN);

    char input[MAX_CMD_LEN];
    pid_t shell_pid = getpid();

  
    if (signal(SIGINT, sigintHandler) == SIG_ERR)
    {
        perror("signal");
        return EXIT_FAILURE;
    }
    if (signal(SIGTSTP, sigtstpHandler) == SIG_ERR)
    {
        perror("signal");
        return EXIT_FAILURE;
    }
  
    char prompt[MAX_PATH_LEN];
    char *username = getpwuid(getuid())->pw_name;
    char *homedir = getpwuid(getuid())->pw_dir;

    while (1)
    {
       
        printf("<%s@%s:%s> ", user, host, cwd);

       
        char input[MAX_CMD_LEN];
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            perror("fgets");
            exit(EXIT_FAILURE);
        }

        input[strcspn(input, "\n")] = '\0';



       if (strcmp(input, "exit") == 0)
        {
           
            break;
        }
        else if (strcmp(input, "Ctrl-C") == 0)
        {
            sigintHandler(SIGINT);
        }
        else if (strcmp(input, "Ctrl-Z") == 0)
        {
            
            sigtstpHandler(SIGTSTP);
        }
        else if (strcmp(input, "Ctrl-D") == 0)
        {
            break;
        }

        int run_in_background = 0;
        if (strlen(input) > 0 && input[strlen(input) - 1] == '&')
        {
            run_in_background = 1;
            input[strlen(input) - 1] = '\0';
        }
        if (strcmp(input, "pwd") == 0)
        {
            printf("~\n");
            continue; 
        }

   
        if (strcmp(input, "activities") == 0)
        {
            listActivities();
            continue;
        }


        if (strchr(input, '|') != NULL || strchr(input, '<') != NULL || strchr(input, '>') != NULL)
        {
            executeCommandWithPipesAndRedirection(input, cwd, shell_home);
            continue;
        }

        if (strchr(input, '|') != NULL)
        {
            executeCommandWithPipes(input, cwd, shell_home);
            continue;
        }

        int input_redirect = 0;
        int output_redirect = 0;
        int append_redirect = 0;
        char input_file[MAX_PATH_LEN] = "";
        char output_file[MAX_PATH_LEN] = "";

        char *input_symbol = strchr(input, '<');
        if (input_symbol != NULL)
        {
            input_redirect = 1;
            sscanf(input_symbol + 1, "%s", input_file);
            *input_symbol = '\0'; 
        }

        char *output_symbol = strstr(input, ">>");
        if (output_symbol != NULL)
        {
            append_redirect = 1;
            sscanf(output_symbol + 2, "%s", output_file);
            *output_symbol = '\0'; 
        }
        else
        {
            output_symbol = strchr(input, '>');
            if (output_symbol != NULL)
            {
                output_redirect = 1;
                sscanf(output_symbol + 1, "%s", output_file);
                *output_symbol = '\0'; 
            }
        }

        if (strncmp(input, "proclore", 8) == 0 && (input[8] == ' ' || input[8] == '\0'))
        {
            char *arg = input + 9; 
            if (strlen(arg) == 0)
            {
                int shell_pid = getpid();
                getProcessInfo(shell_pid, shell_home);
            }
            else
            {
                int pid = atoi(arg);
                if (pid <= 0)
                {
                    printf("Invalid PID: %s\n", arg);
                }
                else
                {
                    getProcessInfo(pid, shell_home);
                }
            }
        }

        if (strncmp(input, "pastevents", 10) != 0)
        {
            add_command(input);
        }

        if (strcmp(input, "pastevents") == 0)
        {
            print_past_commands();
        }
        else if (strstr(input, "pastevents execute") == input)
        {
            int index;
            if (sscanf(input, "pastevents execute %d", &index) == 1)
            {
                execute_past_command(index);
            }
            else
            {
                printf("Invalid command format\n");
            }
        }
        else if (strcmp(input, "pastevents purge") == 0)
        {
            num_commands = 0; }

        else if (strncmp(input, "peek", 4) == 0 && (input[4] == ' ' || input[4] == '\0'))
        {
            char *arg = input + 5; 
            if (strlen(arg) == 0)
            {
                handlePeek(".", cwd, shell_home);
            }
            else
            {
                handlePeek(arg, cwd, shell_home);
            }
        }

        else if (strncmp(input, "warp", 4) == 0 && (input[4] == ' ' || input[4] == '\0'))
        {
            char *arg = input + 5; 
            char curr[1000];
            getcwd(curr, MAX_PATH_LEN);
            if (handleWarp(arg, curr, cwd, shell_home))
            {
                continue; 
            }
            else
            {
                continue;
            }
        }
        else if (strncmp(input, "ping", 4) == 0)
        {
            char *arg = input + 4;
            pingCommand(arg);
        }
        else if (strncmp(input, "fg", 2) == 0) {
         
            char *arg = input + 3; 
            fgCommand(arg);
        }  else if (strncmp(input, "bg", 2) == 0 && (input[2] == ' ' || input[2] == '\0')) {
            char *arg = input + 3; 
            bgCommand(arg);
        }

else if (strncmp(input, "neonate -n", 10) == 0) {
            int time_arg;
            if (sscanf(input + 10, "%d", &time_arg) == 1) {
                neonateCommand(time_arg);
            } else {
                printf("Invalid syntax for neonate command. Usage: neonate -n [time_arg]\n");
            }
        }

        else{
            
            pid_t child_pid = fork();

            if (child_pid == -1)
            {
                perror("fork");
                exit(EXIT_FAILURE);
            }
            else if (child_pid == 0)
            {
                if (run_in_background)
                {
                    freopen("/dev/null", "w", stdout);
                    freopen("/dev/null", "w", stderr);
                }

                execlp("sh", "sh", "-c", input, (char *)NULL);
                perror("execlp");
                exit(EXIT_FAILURE);
            }
            else
            {
                
                struct timeval startTime, endTime;
                gettimeofday(&startTime, NULL);

                if (!run_in_background)
                {
                    int status;
                    waitpid(child_pid, &status, 0);

                    gettimeofday(&endTime, NULL);
                    int elapsedTime = (int)(endTime.tv_sec - startTime.tv_sec);

                    if (elapsedTime > 2)
                    {
                        printf("%s : %ds\n", input, elapsedTime);
                    }
                }
                else
                {
                    struct BackgroundProcess newProcess;
                    newProcess.pid = child_pid;
                    strncpy(newProcess.command, input, sizeof(newProcess.command) - 1);
                    newProcess.startTime = time(NULL);
                    newProcess.status = 0; 
                    backgroundProcesses[numBackgroundProcesses++] = newProcess;

                    printf("[%d]\n", child_pid);
                }
            }

            monitorBackgroundProcesses();
        }

    }

    return 0;
}
