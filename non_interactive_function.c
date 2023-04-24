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
    char **current_command;
    int i;

    while (getline(&line, &n, stdin) != -1)
    {
        remove_newline(line);
        remove_comment(line);
        char **commands = tokenizer(line, ";");
        for (i = 0; commands[i] != NULL; i++)
        {
            *current_command = tokenizer(commands[i], " ");
            if (current_command[0] != NULL)
            {
                int type_command = parse_command(current_command[0]);
                initializer(current_command, type_command);
            }
            free(current_command);
        }
        free(commands);
    }
    free(line);
    exit(status);
}

