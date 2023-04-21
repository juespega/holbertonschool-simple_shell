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
    
    while (1) {
        printf("ana,juliyrubio> ");

        
        char_read = getline(&line, &bytes_read, stdin);

        if (char_read == -1) {
           return (-1);
        }

        
        line[strcspn(line, "\n")] = '\0';

        
        token = strtok(line, " ");

        if (token == NULL) {
            continue;
        }

        
        pid = fork();

        if (pid == -1) {
            return (-1);
        } else if (pid == 0) {
            
            if (execlp(token, token, (char *) NULL) == -1) {
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
