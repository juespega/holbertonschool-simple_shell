#include "shell.h"
#define MAX_COMMAND 10

void prompt(char **av __attribute__((unused)), char **env)
{
    char *str = NULL;
    int i, j, status, last_cmd_exit_status = 0;
    size_t n = 0;
    ssize_t num_char;
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
        num_char = getline(&str, &n, stdin);
        if (num_char == -1)
        {
            free(str);
            exit (WEXITSTATUS(status));
        }
        i = 0;
        while (str[i])
        {
            if (str[i] == '\n')
            {
                str[i] = 0;
            }
            i++;
        }

        if (strcmp(argv[0], "clear") == 0)
        {
            system("clear");
            continue;
        }

        if (strcmp(argv[0], "exit") == 0)
        {
            free(str);
            exit(last_cmd_exit_status);
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
            free(str);
            exit(1);
        }
        if (pid == 0)
        {
            path = getenv("PATH");
            j = 0;
            argv[j] = strtok(str, " ");
            while (argv[j])
            {
                argv[++j] = strtok(NULL, " ");
            }
            
            if ((argv[0] == NULL) || strlen(argv[0]) == 0)
            {
                continue;
            }

            if (execve(argv[0], argv, env) == -1)
            {
                if (path != NULL)
                {
                    token = strtok(path, ":");
                    while (token != NULL)
                    {
                        cmd_path = malloc(strlen(token) + strlen(argv[0]) + 2);
                        sprintf(cmd_path, "%s/%s", token, argv[0]);
                        if (access(cmd_path, F_OK) == 0)
                        {
                            argv[0] = cmd_path;
                            execve(argv [0], argv, env);
                        }
                        else
                        {
                            free(cmd_path);
                            token = strtok(NULL, ":");
                        }
                    }
                }
                /* Print an error message if the command is not found */
                fprintf(stderr, "./hsh: 1: %s: not found\n", argv[0]);
                free(str);
                exit(127);
            }
        }
        else
        {
            waitpid(pid, &status, 0);
            if (WIFEXITED(status))
            {
                last_cmd_exit_status = WEXITSTATUS(status);
            }
        }
    }
}

