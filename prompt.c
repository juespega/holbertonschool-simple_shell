#include "shell.h"
#define MAX_COMPAND 10
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
    size_t n = 0;
    int i, j;
    int status;
    ssize_t num_char;
    char *argv[MAX_COMPAND];
    pid_t chil_pid;
    extern char **environ;
    char *path_copy;
    char *dir;
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
        while (argv[j])
            argv[++j] = strtok(NULL, " ");
        chil_pid = fork();
        if (chil_pid == -1)
        {
            free(string);
            exit(EXIT_FAILURE);
        }
        if (chil_pid == 0)
        {
            if ((argv[0] == NULL) || strlen(argv[0]) == 0)
            {
                continue;
            }
            if (execve(argv[0], argv, env) == -1)
            {
                /**Verificar si el comando existe en las rutas especificadas en PATH**/
                char *path_value = NULL;
                char *path_name = "PATH=";
                char **envp = environ;
                while (*envp != NULL) {
                    if (strncmp(*envp, path_name, strlen(path_name)) == 0) {
                        path_value = strchr(*envp, '=') + 1;
                        break;
                    }
                    envp++;
                }
                if (path_value == NULL) {
                    printf("No se encontró la variable de entorno PATH.\n");
                    exit(EXIT_FAILURE);
                }
                path_copy = strdup(path_value);
                dir = strtok(path_copy, ":");
                while (dir)
                {
                    char *cmd_path = malloc(strlen(dir) + strlen(argv[0]) + 2);
                    sprintf(cmd_path, "%s/%s", dir, argv[0]);
                    if (access(cmd_path, X_OK) == 0)
                    {
                        /**Ejecutar el comando si existe en PATH**/
                        execve(cmd_path, argv, env);
                    }
                    free(cmd_path);
                    dir = strtok(NULL, ":");
                }
                printf("%s: No funciona con este comando \n ", av[0]);
                free(path_copy);
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            wait(&status);
        }
    }
}  
