#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

char* tokens[MAX_NUM_TOKENS];

void tokenize(char* input)
{
    int token_count = 0;
    int token_size = 0;
    char* cursor = input;

    while (*cursor != '\0')
    {
        if (*cursor == ' ' || *cursor == '\n' || *cursor == '\t')
        {
            if (token_size > 0)
            {
                tokens[token_count] = (char*)malloc(sizeof(char) * (token_size + 1));
                token_size = 0;
                token_count++;
            }
        } else
        {
            tokens[token_count][token_size] = *cursor;
            token_size++;
        }
        cursor++;
    }
    if (token_size > 0)
    {
        tokens[token_count] = (char*)malloc(sizeof(char) * (token_size + 1));
        token_count++;
    }
    tokens[token_count] = NULL;
}

int main()
{
    char input[MAX_INPUT_SIZE];
    while (1) {
        int i;
        printf(":) ");
        fgets(input, MAX_INPUT_SIZE, stdin);
        tokenize(input);
        if (tokens[0] != NULL)
        {
            int pid = fork();
            if (pid == 0)
            {
                if (execvp(tokens[0], tokens) == -1)
                {
                    printf("Error: no se pudo ejecutar el comando\n");
                }
                exit(EXIT_FAILURE);
            } else if (pid < 0)
            {
                printf("Error: fallo en la creación del proceso hijo\n");
            } else
            {
                wait(NULL);
            }
        }
        for (i = 0; tokens[i] != NULL; i++)
        {
            free(tokens[i]);
            tokens[i] = NULL;
        }
    }
    return 0;
}
