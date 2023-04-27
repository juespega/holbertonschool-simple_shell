#include "shell.h"

int main(int ac, char **av, char **env)
{
  if (ac == 1)
    {
      prompt(av, env);

    }
  return 0;
}

/**
 * env_builtin - prints all of the program's environment variables to the console.
 * @env: Array of strings contains the environment variables of a program in the form of key=value 
 *
 * Description: contains the environment variables of a program in the form of key=value
 * Return: 0
 */


int env_builtin(char **env)
{
    int i = 0;
    while (env[i] != NULL)
    {
        printf("%s\n", env[i]);
        i++;
    }
    return (0);
}
