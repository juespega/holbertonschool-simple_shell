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
    char *line = NULL;
    int i, j, status, cmd_exit_status = 0;
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
            printf("juli,anayrubio ");
            fflush(stdout);
        }
        num_char = getline(&line, &n, stdin);
        if (num_char == -1)
        {
            free(line);
            cmd_exit_status = WEXITSTATUS(status);
        }
        i = 0;
        while (line[i])
        {
            if (line[i] == '\n')
            {
                line[i] = 0;
            }
            i++;
        }

        path = getenv("PATH");
        j = 0;
        argv[j] = strtok(line, " ");
        while (argv[j] != NULL)
        {
            argv[++j] = strtok(NULL, " ");
        }

        if (strcmp(argv[0], "exit") == 0)
        {
            free(line);
            exit(cmd_exit_status);
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
            free(line);
            exit(1);
        }
        if (pid == 0)
        {
            if ((argv[0] == NULL) || strlen(argv[0]) == 0)
            {
              free(line);
                exit(cmd_exit_status);
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
                free(line);
                exit(127);
            }
        }
        else
        {
            waitpid(pid, &status, 0);
            if (WIFEXITED(status))
            {
                cmd_exit_status = WEXITSTATUS(status);
            }
        }
    }
}

