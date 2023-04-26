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
    int i, j, status = 0;
    char *string = NULL, *argv[MAX_COMPAND];
    char *path_copy, *dir, *path_aux;
    char *path_value = NULL;
    char *path_name = "PATH=";
    char *cmd_path = NULL;
    extern char **environ;
    char **envp = environ;
    size_t n = 0;
    ssize_t num_char;
    pid_t chil_pid;
    
    while (1)
    {
        if (isatty(STDIN_FILENO))
            printf("cisfun$ ");
        num_char = getline(&string, &n, stdin);
        if (num_char == -1)
        {
            free(string);
            exit(WEXITSTATUS(status));
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
        {   /*este código se corta*/
            if ((argv[0] == NULL) || strlen(argv[0]) == 0)
            {
                continue;
            }
            if (strcmp(argv[0], "exit") == 0)
            {
            free(string);
            exit(WEXITSTATUS(status));
            }
            if (strcmp(argv[0], "env") == 0)
            {
            env_builtin(env);
            continue;
            }


            if (execve(argv[0], argv, env) == -1)
            {
                /**Verificar si el comando existe en las rutas especificadas en PATH**/
                
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
                path_aux = path_copy;
                dir = strtok(path_copy, ":");
                while (dir)
                {
                    cmd_path = malloc(strlen(dir) + strlen(argv[0]) + 2);
                    sprintf(cmd_path, "%s/%s", dir, argv[0]);
                    if (access(cmd_path, X_OK) == 0)
                    {
                        /**Ejecutar el comando si existe en PATH**/
                        argv[0] = cmd_path;
                        free(path_aux);
                        break;
                    }
                    free(cmd_path);
                    cmd_path = NULL;
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
