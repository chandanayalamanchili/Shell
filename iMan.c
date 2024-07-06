#include"headers.h"
extern int numBackgroundProcesses;

extern pid_t foregroundProcessPid;
extern int isForegroundRunning;
extern int num_commands;
 extern struct BackgroundProcess backgroundProcesses[MAX_BACKGROUND_PROCESSES];
extern pid_t shellProcesses[MAX_BACKGROUND_PROCESSES];

extern char past_commands[MAX_HISTORY][MAX_CMD_LEN];


void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void print_man_page(const char *content)
{
    char *content_copy = strdup(content); // Create a mutable copy
    if (content_copy == NULL)
    {
        perror("Error: Memory allocation failed");
        return;
    }

    char *line = strtok(content_copy, "\n");
    while (line != NULL)
    {
        printf("%s\n", line);
        line = strtok(NULL, "\n");
    }

    free(content_copy); // Free the copy when done
}


void fetchAndDisplayManPage(const char *command_name) {
    struct hostent *server;
    struct sockaddr_in server_addr;
    int sockfd, bytes_read;
    char request[MAX_RESPONSE_SIZE];
    char response[MAX_RESPONSE_SIZE];
    int is_content = 0;
    char content[MAX_RESPONSE_SIZE];

    // Initialize content as an empty string
    content[0] = '\0';

    server = gethostbyname("man.he.net");
    if (server == NULL)
    {
        error("Error: Cannot resolve hostname.");
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error("Error: Cannot open socket.");
    }

    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80);
    bcopy((char *)server->h_addr, (char *)&server_addr.sin_addr.s_addr, server->h_length);

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        error("Error: Cannot connect to server.");
    }

    snprintf(request, sizeof(request), "GET /?topic=%s&section=all HTTP/1.0\r\n\r\n", command_name);

    if (write(sockfd, request, strlen(request)) < 0)
    {
        error("Error: Cannot send request to server.");
    }

    bzero(response, sizeof(response));

    while ((bytes_read = read(sockfd, response, sizeof(response))) > 0)
    {
        for (int i = 0; i < bytes_read; i++)
        {
            if (response[i] == '<')
            {
                is_content = 0;
                continue;
            }
            else if (response[i] == '>')
            {
                is_content = 1;
                continue;
            }

            if (is_content && strlen(content) < MAX_RESPONSE_SIZE - 1)
            {
                strncat(content, &response[i], 1);
            }
        }

        bzero(response, sizeof(response));
    }

    close(sockfd);

    // Check if the content contains an error message
    if (strstr(content, "No such command") != NULL)
    {
        printf("ERROR\nNo such command\n");
    }
    else
    {
        print_man_page(content);
    }
}

void iManCommand(const char *arg) {
    printf("Fetching man page for: %s\n", arg);
    fetchAndDisplayManPage(arg);
}