#include "shell.h"
/**
 * initializer - starts executing everything
 * @current_command: try to check current token
 * @type_command: parse token
 *
 * Return: void function
 */

void initializer(char **current_command, int type_command)
{
	if (type_command == EXTERNAL_COMMAND || type_command == PATH_COMMAND)
	{
		if (fork() == 0)
			execute_command(current_command, type_command);
		else
			execute_command(current_command, waitpid(-1, NULL, 0) >> 8);
	}
	else
		execute_command(current_command, type_command);
}
