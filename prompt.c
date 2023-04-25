#include "shell.h"
#define MAX_COMMANDS 10

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
    char *argv[MAX_COMMANDS];
    pid_t child_pid;
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
            if (execve(argv[0], argv, env) == -1)
            {
                /* Verificar si el comando existe en las rutas especificadas en PATH */
                char *path_value = NULL;
                char *path_name = "PATH=";
                char **envp = environ;

                while (*envp != NULL)
                {
                    if (strncmp(*envp, path_name, strlen(path_name)) == 0)
                    {
                        path_value = strchr(*envp, '=') + 1;
                        break;
                    }
                    envp++;
                }

                if (path_value == NULL)
                {
                    printf("No se encontró la variable de entorno PATH.\n");
                    exit(EXIT_FAILURE);
                }

                path_copy = strdup(path_value);
                dir = strtok(path_copy, ":");

                while (dir)
                {
                    struct stat sb;
                    char cmd_path[BUFSIZ];
                    int len = snprintf(cmd_path, BUFSIZ, "%s/%s", dir, argv[0]);

                    if (len >= BUFSIZ)
                    {
                        printf("Error: la ruta del comando es demasiado larga.\n");
                        exit(EXIT_FAILURE);
                    }

                    if (stat(cmd_path, &sb) == -1)
                    {
                        dir = strtok(NULL, ":");
                        continue;
                    }

                    if (S_ISREG(sb.st_mode) && (sb.st_mode & S_IXUSR))
                    {
                        /* Ejecutar el comando si existe en PATH */
                        execve(cmd_path, argv, env);
                    }

                    dir = strtok(NULL, ":");
                }

                printf("%s: No se encontró el comando.\n", argv[0]);
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

