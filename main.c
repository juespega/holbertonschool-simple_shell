#include "main.h"

int ran = 1;
int main()
{
    int res;

    char *args[BUFFER_SIZE];

    char line[MAX_LINE_LENGTH];


    char t_line[MAX_LINE_LENGTH];

    char history[MAX_LINE_LENGTH] = "No commands in history";


    char *redir_argv[REDIR_SIZE];


    int wait;


    void init_shell();

    res = 0;

    while (ran == 1)
    {

        printf("%s:%s> ", prompt(), get_current_dir());
        fflush(stdout);


        read_line(line);


        strcpy(t_line, line);


        parse_command(line, args, &wait);


        if (strcmp(args[0], "!!") == 0)
        {
            res = simple_shell_history(history, redir_argv);
        }
        else
        {
            set_prev_command(history, t_line);
            exec_command(args, redir_argv, wait, res);
        }
        res = 0;
    }
    return 0;
}
