#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>

#define MAX_LINE_LENGTH 1024
#define BUFFER_SIZE 64
#define REDIR_SIZE 2
#define PIPE_SIZE 3
#define MAX_HISTORY_SIZE 128
#define MAX_COMMAND_NAME_LENGTH 128
#define PROMPT_FORMAT "%Y-%m-%d %H:%M:%S "
#define PROMPT_MAX_LENGTH 30

#define TOFILE_DIRECT ">"
#define APPEND_TOFILE_DIRECT ">>"
#define FROMFILE "<"
#define PIPE_OPT "|"



char *get_current_dir();
char *prompt();
void error_alert(char *msg);
void remove_end_of_line(char *line);
void read_line(char *line);
void parse_command(char *input_string, char **argv, int *wait);
int is_redirect(char **argv);
int is_pipe(char **argv);
void parse_redirect(char **argv, char **redirect_argv, int redirect_index);
void parse_pipe(char **argv, char **child01_argv, char **child02_argv, int pipe_index);
void exec_child(char **argv);
void exec_child_overwrite_from_file(char **argv, char **dir);
void exec_child_overwrite_to_file(char **argv, char **dir);
void exec_child_append_to_file(char **argv, char **dir);
void exec_child_pipe(char **argv_in, char **argv_out);
void set_prev_command(char *history, char *line);
char *get_prev_command(char *history);
int simple_shell_cd(char **args);
int simple_shell_help(char **args);
int simple_shell_exit(char **args);
void exec_command(char **args, char **redir_argv, int wait, int res);
int simple_shell_num_builtins();
int simple_shell_history(char *history, char **redir_args);
int simple_shell_redirect(char **args, char **redir_argv);
int simple_shell_pipe(char **args);
int main();
#endif
