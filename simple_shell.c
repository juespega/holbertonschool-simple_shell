#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

int main(void) 
{
    ssize_t char_read;
    char *line = NULL;
    size_t bytes_read = 0;
    char *token;
    pid_t pid;
    char *tokens;
    while (1) {
        printf("ajr> ");

        char_read = getline(&line, &bytes_read, stdin);

        if (char_read == -1) {
           return (-1);
        }

        line[strcspn(line, "\n")] = '\0';

      
        tokens[BUFFER_SIZE];
        int num_tokens = 0;
        token = strtok(line, " \n");
        while (token != NULL) {
            tokens[num_tokens] = token;
            num_tokens++;
            token = strtok(NULL, " \n");
        }
        tokens[num_tokens] = NULL;

        if (num_tokens == 0) {
            continue;
        }

        pid = fork();

        if (pid == -1) {
            return (-1);
        } else if (pid == 0) {
           
            if (execvp(tokens[0], tokens) == -1) {
                perror("Error");
                return (-1);
            }
        } else {
            int status;
            if (wait(&status) == -1) {
                return (-1);
            }
        }
    }

    free(line);

    return 0;
}
