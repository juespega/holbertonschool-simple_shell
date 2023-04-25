#include "shell.h"

int main(int ac, char **av, char **env)
{
  if (ac == 1)
    {
      prompt(env);

    }
  return 0;
}
