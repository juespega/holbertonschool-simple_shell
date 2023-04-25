#include "shell.h"

/**
 * tokenizer - tokenizes input and stores it into an array
 * @input_string: input to be parsed
 * @delim: delimiter to be used, needs to be one character string
 *
 * Return: array of tokens
 */
char **tokenizer(char *input_string, char *delim)
{
	int num_delim = 0;
	char **av = NULL, *token = NULL, *save_ptr = NULL;

	token = _strtok_r(input_string, delim, &save_ptr);

	while (token)
	{
		av = _realloc(av, sizeof(*av) * num_delim, sizeof(*av) * (num_delim + 1));
		av[num_delim++] = token;
		token = _strtok_r(NULL, delim, &save_ptr);
	}
	av = _realloc(av, sizeof(*av) * num_delim, sizeof(*av) * (num_delim + 1));
	av[num_delim] = NULL;

	return (av);
}

/**
 * print - prints a string to stdout
 * @string: string to be printed
 * @stream: stream to print out to
 */
void print(char *string, int stream)
{
	for (; *string; string++)
		write(stream, string, 1);
}

/**
 * remove_newline - removes new line from a string
 * @str: string to be used
 */
void remove_newline(char *str)
{
	for (; *str; str++)
	{
		if (*str == '\n')
			break;
	}
	*str = '\0';
}

/**
 * _strcpy - copies a string to another buffer
 * @src: source to copy from
 * @dest: destination to copy to
 */
void _strcpy(char *src, char *dest)
{
	while ((*dest++ = *src++))
		;
}

/**
 * _strlen - counts string length
 * @string: string to be counted
 * Return: length of the string
 */
int _strlen(char *string)
{
	int len = 0;
	for (; string && string[len]; len++)
		;
	return (len);
}

