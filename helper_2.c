#include "shell.h"

char *_strtok_r(char *s, char *delim, char **save_ptr)
{
	if (s == NULL)
		s = *save_ptr;

	s += _strspn(s, delim);

	if (*s == '\0')
	{
		*save_ptr = s;
		return (NULL);
	}

	*save_ptr = s + _strcspn(s, delim);

	if (**save_ptr != '\0')
		*(*save_ptr)++ = '\0';

	return (s);
}

int _atoi(char *s)
{
	unsigned int n = 0;

	do {
		if (*s == '-')
			return (-1);
		else if ((*s < '0' || *s > '9') && *s != '\0')
			return (-1);
		else if (*s >= '0'  && *s <= '9')
			n = (n * 10) + (*s - '0');
		else if (n > 0)
			break;
	} while (*s++);

	return (n);
}

void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
	void *temp_block;
	unsigned int i;

	if (ptr == NULL)
	{
		temp_block = malloc(new_size);
		return (temp_block);
	}

	if (new_size == old_size)
		return (ptr);

	if (new_size == 0 && ptr != NULL)
	{
		free(ptr);
		return (NULL);
	}

	temp_block = malloc(new_size);

	if (temp_block == NULL)
		return (NULL);

	for (i = 0; i < min(old_size, new_size); i++)
		*((char *)temp_block + i) = *((char *)ptr + i);

	free(ptr);
	return (temp_block);
}

void ctrl_c_handler(int signum)
{
	if (signum == SIGINT)
		print("\n($) ", STDIN_FILENO);
}

void remove_comment(char *input)
{
	int i = 0;

	if (input[i] == '#')
		input[i] = '\0';

	while (input[i] != '\0')
	{
		if (input[i] == '#' && input[i - 1] == ' ')
			break;
		i++;
	}

	input[i] = '\0';
}
