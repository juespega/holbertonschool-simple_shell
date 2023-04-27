#include "shell.h"
#define MAX_COMMAND 10

/**
 * prompt - Display a shell prompt and execute commands
 * @av: Array of command line argument strings
 * @env: Array of environment variable strings
 * Description: This function displays a shell prompt
 * Return: None
 */

void prompt(char **av __attribute__((unused)), char **env)
{
    char *string = NULL;
    int j, status, exit_status = 0;
    ssize_t len;
    size_t n = 0;
    char *argv[MAX_COMMAND];
    char *path, *cmd_path, *token;
    char **ptr;
    pid_t pid;

    while (1)
    {
        if (isatty(STDIN_FILENO))
        {
            printf("$ ");
            fflush(stdout);
        }

  
        signal(SIGINT, SIG_DFL);

        
        memset(argv, 0, sizeof(argv));
        free(string);
        string = NULL;
        n = 0;

        
        len = getline(&string, &n, stdin);
        if (len == -1)
        {
            free(string);
            exit(exit_status);
        }

       
        string[strcspn(string, "\n")] = '\0';

      
        path = getenv("PATH");
        j = 0;
        argv[j] = strtok(string, " ");
        while (argv[j] != NULL && j < MAX_COMMAND - 1)
        {
            argv[++j] = strtok(NULL, " ");
        }

       
        if (strcmp(argv[0], "clear") == 0)
        {
            system("clear");
            continue;
        }
        if (strcmp(argv[0], "exit") == 0)
        {
            free(string);
            exit(exit_status);
        }
        if (strcmp(argv[0], "env") == 0)
        {
            ptr = env;
            while (*ptr != NULL)
            {
                printf("%s\n", *ptr);
                ptr++;
            }
            continue;
        }

      
        pid = fork();
        if (pid == -1)
        {
            free(string);
            exit(1);
        }
        if (pid == 0)
        {
            
            if ((argv[0] == NULL) || strlen(argv[0]) == 0)
            {
                free(string);
                exit(EXIT_SUCCESS);
            }

            
            if (access(argv[0], F_OK) != 0 && path != NULL)
            {
                token = strtok(path, ":");
                while (token != NULL)
                {
                    cmd_path = malloc(strlen(token) + strlen(argv[0]) + 2);
                    sprintf(cmd_path, "%s/%s", token, argv[0]);
                    if (access(cmd_path, F_OK) == 0)
                    {
                        argv[0] = cmd_path;
                        execve(argv[0], argv, env);
                    }
                    else
                    {
                        free(cmd_path);
                        token = strtok(NULL, ":");
                    }
                }
            }

           
            if (execve(argv[0], argv, env) == -1)
            {
                fprintf(stderr, "./hsh: %d: %s: not found\n", getpid(), argv[0]);
                free(string);
                exit(127);
            }
        }
        else
        {
            waitpid(pid, &status, 0);
            if (WIFSIGNALED(status) && WTERMSIG(status) == SIGSEGV)
                {
                    printf("Segmentation fault\n");
                    exit_status = 139;
                }
            else if (WIFEXITED(status))
                {
                    exit_status = WEXITSTATUS(status);
                }

           
        }
        free(string);
        string = NULL;
        n = 0;
    }
}
