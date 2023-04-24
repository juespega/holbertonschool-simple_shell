#include "shell.h"

/**
 * non_interactive - handles non_interactive mode
 *
 * Return: void
 */

void non_interactive(void)
{
    char *line = NULL;
    size_t n = 0;
    char **current_command = NULL; // inicializar a NULL
    char **commands;
    int i;
    int type_command;

    while (getline(&line, &n, stdin) != -1)
    {
        remove_newline(line);
        remove_comment(line);
        commands = tokenizer(line, ";");
        for (i = 0; commands[i] != NULL; i++)
        {
            current_command = tokenizer(commands[i], " ");
            if (current_command != NULL && current_command[0] != NULL)
            {
                type_command = parse_command(current_command[0]);
                initializer(current_command, type_command);
            }
            if (current_command != NULL) // asegurarse de que no se libere NULL
                free(current_command);
        }
        free(commands);
    }
    if (line != NULL) // asegurarse de que no se libere NULL
        free(line);
    exit(status);
}
