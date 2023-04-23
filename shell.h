#ifndef SHELL_H
#define SHELL_H
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define MAX_COMMAND 10
#define MAX_PATH_LEN 1024
#define MAX_COMPAND 10

void prompt(char **av, char **env);
int main(int ac, char **av, char **env);

#endif
