#include "shell.h"

#define MAX_COMMAND 10
/**
 * prompt - Display a shell prompt and execute commands
 * @av: Array of command line argument strings
 * @env: Array of environment variable strings
 * Description: This function displays a shell prompt
 * Return: None
 */
void prompt(char **av, char **env)
{
    char *string = NULL;
    int i, j, status;
    size_t n = 0;
    ssize_t num_char;
    char *argv[MAX_COMMAND];
    pid_t child_pid;
    while (1)
    {
        if (isatty(STDIN_FILENO))
            printf("cisfun$ ");
        num_char = getline(&string, &n, stdin);
        if (num_char == -1)
        {
            free(string);
            exit(EXIT_SUCCESS);
        }
        
        i = 0;
        while (string[i])
        {
            if (string[i] == '\n')
                string[i] = 0;
            i++;
        }
        j = 0;
        argv[j] = strtok(string, " ");
        while (argv[j] != NULL)
        {
            argv[++j] = strtok(NULL, " ");
        }
        child_pid = fork();
        if (child_pid == -1)
        {
            free(string);
            exit(EXIT_FAILURE);
        }
        if (child_pid == 0)
        {
            if ((argv[0] == NULL) || strlen(argv[0]) == 0)
            {
                continue;
            }
            else if (execve(argv[0], argv, env) == -1)
            {
                printf("%s: No such file or directory\n", av[0]);
            }
        }
        else
            wait(&status);
    }
}
