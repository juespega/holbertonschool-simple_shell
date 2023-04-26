#include "shell.h"

int main(int ac, char **av, char **env)
{
  if (ac == 1)
    {
      prompt(av, env);

    }
  return 0;
}
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
