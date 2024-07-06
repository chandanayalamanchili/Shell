struct BackgroundProcess
{
    pid_t pid;
    char command[MAX_CMD_LEN];
    time_t startTime;
    int status; // 0: Running, 1: Completed normally, 2: Completed abnormally
};
// struct BackgroundProcess backgroundProcesses[MAX_BACKGROUND_PROCESSES];