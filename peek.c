#include"headers.h"

extern int numBackgroundProcesses;

extern pid_t foregroundProcessPid;
extern int isForegroundRunning;
extern int num_commands;
extern struct BackgroundProcess backgroundProcesses[MAX_BACKGROUND_PROCESSES];
extern pid_t shellProcesses[MAX_BACKGROUND_PROCESSES];

extern char past_commands[MAX_HISTORY][MAX_CMD_LEN];

// Function to check if a path exists
int pathExists(const char *path)
{
    struct stat buffer;
    return (stat(path, &buffer) == 0);
}

// Function to list files and directories in a directory
void listDirectory(const char *path, int showHidden, int showDetails)
{
    DIR *dir = opendir(path);
    if (dir == NULL)
    {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (!showHidden && entry->d_name[0] == '.')
        {
            continue; // Skip hidden files
        }

        char fullpath[MAX_PATH_LEN];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);

        struct stat st;
        if (lstat(fullpath, &st) < 0)
        {
            perror("lstat");
            exit(EXIT_FAILURE);
        }

        char type;
        if (S_ISDIR(st.st_mode))
        {
            type = 'd';
        }
        else if (S_ISLNK(st.st_mode))
        {
            type = 'l';
        }
        else if (S_ISREG(st.st_mode))
        {
            type = '-';
        }
        else
        {
            type = '?';
        }

        if (showDetails)
        {
            struct passwd *pwd = getpwuid(st.st_uid);
            struct group *grp = getgrgid(st.st_gid);
            char time_str[80];
            strftime(time_str, sizeof(time_str), "%b %d %Y %H:%M", localtime(&st.st_mtime));

            printf("%c%s %s %s %lld %s %s\n",
                   type,
                   (st.st_mode & S_IRUSR) ? "r" : "-",
                   (st.st_mode & S_IWUSR) ? "w" : "-",
                   (st.st_mode & S_IXUSR) ? "x" : "-",
                   (long long)st.st_size,
                   time_str,
                   entry->d_name);
        }
        else
        {
            // Use ANSI color codes for different file types
            const char *color_code = (type == 'd') ? "\x1B[34m" : ((type == 'l') ? "\x1B[32m" : "\x1B[0m");
            printf("%s%s\x1B[0m\n", color_code, entry->d_name);
        }
    }

    closedir(dir);
}

void handlePeek(const char *arg, char cwd[], char shell_home[])
{
    int showHidden = 0;
    int showDetails = 0;
    char curr[MAX_PATH_LEN];

    if (strcmp(cwd, "~") == 0)
    {
        strcpy(curr, shell_home);
    }
    else
    {
        strcpy(curr, shell_home);
        strcat(curr, "/");
        strcat(curr, cwd);
    }

    // Check for peek flags
    char *path = NULL;
    char *token = strtok((char *)arg, " ");
    while (token != NULL)
    {
        if (strcmp(token, "-a") == 0)
        {
            showHidden = 1;
        }
        else if (strcmp(token, "-l") == 0)
        {
            showDetails = 1;
        }
        else
        {
            path = token;
        }
        token = strtok(NULL, " ");
    }

    char fullpath[MAX_PATH_LEN * 2]; // Double the buffer size

    // Check if the path exists in the current directory
    snprintf(fullpath, sizeof(fullpath), "%s/%s", curr, path);
    if (pathExists(fullpath))
    {
        listDirectory(fullpath, showHidden, showDetails);
        return;
    }

    snprintf(fullpath, sizeof(fullpath), "%s/%s", curr, path);
    if (pathExists(fullpath))
    {
        listDirectory(fullpath, showHidden, showDetails);
        return;
    }

    
    printf("Path '%s' does not exist in the current directory or one level up.\n", path);
}