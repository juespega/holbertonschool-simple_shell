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

    while (getline(&line, &n, stdin) != -1)
    {
        remove_newline(line);
        remove_comment(line);
        char **commands = tokenizer(line, ";");
        for (int i = 0; commands[i] != NULL; i++)
        {
            char **current_command = tokenizer(commands[i], " ");
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

