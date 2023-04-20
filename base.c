#include "main.h"

char *get_current_dir()
{
		char cwd[FILENAME_MAX];
		char*result;
		result = getcwd(cwd, sizeof(cwd));
		return result;
}



char *prompt()
{
		char* username;
		static char *_prompt = NULL;
		time_t now;
		struct tm *tmp;
		size_t size;

		if (_prompt == NULL) {
				_prompt = malloc(PROMPT_MAX_LENGTH * sizeof(char));
				if (_prompt == NULL)
				{
						perror("Error: Unable to locate memory");
						exit(EXIT_FAILURE);
				}
		}


		now = time(NULL);
		if (now == -1)
		{
				fprintf(stderr, "Error: Cannot get current timestamp");
				exit(EXIT_FAILURE);
		}


		tmp = localtime(&now);
		if (tmp == NULL)
		{
				fprintf(stderr, "Error: Cannot identify timestamp");
				exit(EXIT_FAILURE);
		}


		size = strftime(_prompt, PROMPT_MAX_LENGTH, PROMPT_FORMAT, tmp);
		if (size == 0)
		{
				fprintf(stderr, "Error: Cannot convert time to string");
				exit(EXIT_FAILURE);
		}

		username = getenv("USER");
		strncat(_prompt, username, strlen(username));
		return _prompt;
}


void error_alert(char *msg)
{
		printf("%s %s\n", prompt(), msg);
}


void remove_end_of_line(char *line)
{
		int i = 0;
		while (line[i] != '\n')
		{
				i++;
		}

		line[i] = '\0';
}


void read_line(char *line)
{
		char *ret;
		ret = fgets(line, MAX_LINE_LENGTH, stdin);


		remove_end_of_line(line);


		if (strcmp(line, "exit") == 0 || ret == NULL || strcmp(line, "quit") == 0)
		{
				exit(EXIT_SUCCESS);
		}
}



void parse_command(char *input_string, char **argv, int *wait)
{
		int i = 0;

		while (i < BUFFER_SIZE)
		{
				argv[i] = NULL;
				i++;
		}


		*wait = (input_string[strlen(input_string) - 1] == '&') ? 0 : 1;
		input_string[strlen(input_string) - 1] = (*wait == 0) ? input_string[strlen(input_string) - 1] = '\0' : input_string[strlen(input_string) - 1];
		i = 0;
		argv[i] = strtok(input_string, " ");

		if (argv[i] == NULL) return;

		while (argv[i] != NULL)
		{
				i++;
				argv[i] = strtok(NULL, " ");
		}

		argv[i] = NULL;
}


int is_redirect(char **argv)
{
		int i = 0;
		while (argv[i] != NULL)
		{
				if (strcmp(argv[i], TOFILE_DIRECT) == 0 || strcmp(argv[i], APPEND_TOFILE_DIRECT) == 0 || strcmp(argv[i], FROMFILE) == 0)
				{
						return i;
				}
				i = -~i;
		}
		return 0;
}


int is_pipe(char **argv)
{
		int i = 0;
		while (argv[i] != NULL)
		{
				if (strcmp(argv[i], PIPE_OPT) == 0)
				{
						return i;
				}
				i = -~i;
		}
		return 0;
}


void parse_redirect(char **argv, char **redirect_argv, int redirect_index)
{
		redirect_argv[0] = strdup(argv[redirect_index]);
		redirect_argv[1] = strdup(argv[redirect_index + 1]);
		argv[redirect_index] = NULL;
		argv[redirect_index + 1] = NULL;
}



void parse_pipe(char **argv, char **child01_argv, char **child02_argv, int pipe_index)
{
		int i = 0;
		for (i = 0; i < pipe_index; i++)
		{
				child01_argv[i] = strdup(argv[i]);
		}
		child01_argv[i++] = NULL;

		while (argv[i] != NULL)
		{
				child02_argv[i - pipe_index - 1] = strdup(argv[i]);
				i++;
		}
		child02_argv[i - pipe_index - 1] = NULL;
}


void exec_child(char **argv)
{
		if (execvp(argv[0], argv) < 0)
		{
				fprintf(stderr, "Error: Failed to execte command.\n");
				exit(EXIT_FAILURE);
		}
}


void exec_child_overwrite_from_file(char **argv, char **dir)
{
		int fd_in;
		fd_in = open(dir[1], O_RDONLY);
		if (fd_in == -1)
		{
				perror("Error: Redirect input failed");
				exit(EXIT_FAILURE);
		}

		dup2(fd_in, STDIN_FILENO);

		if (close(fd_in) == -1)
		{
				perror("Error: Closing input failed");
				exit(EXIT_FAILURE);
		}
		exec_child(argv);
}


void exec_child_overwrite_to_file(char **argv, char **dir)
{
		int fd_out;
		fd_out = creat(dir[1], S_IRWXU);
		if (fd_out == -1)
		{
				perror("Error: Redirect output failed");
				exit(EXIT_FAILURE);
		}
		dup2(fd_out, STDOUT_FILENO);
		if (close(fd_out) == -1)
		{
				perror("Error: Closing output failed");
				exit(EXIT_FAILURE);
		}

		exec_child(argv);
}


void exec_child_append_to_file(char **argv, char **dir)
{

		int fd_out;
		if (access(dir[0], F_OK) != -1)
		{
				fd_out = open(dir[0], O_WRONLY | O_APPEND);
		}
		if (fd_out == -1)
		{
				perror("Error: Redirect output failed");
				exit(EXIT_FAILURE);
		}
		dup2(fd_out, STDOUT_FILENO);
		if (close(fd_out) == -1)
		{
				perror("Error: Closing output failed");
				exit(EXIT_FAILURE);
		}
		exec_child(argv);
}


void exec_child_pipe(char **argv_in, char **argv_out)
{
		int fd[2];

		if (pipe(fd) == -1)
		{
				perror("Error: Pipe failed");
				exit(EXIT_FAILURE);
		}


		if (fork() == 0)
		{
				dup2(fd[1], STDOUT_FILENO);
				close(fd[0]);
				close(fd[1]);
				exec_child(argv_in);
				exit(EXIT_SUCCESS);
		}


		if (fork() == 0)
		{
				dup2(fd[0], STDIN_FILENO);
				close(fd[1]);
				close(fd[0]);
				exec_child(argv_out);
				exit(EXIT_SUCCESS);
		}

		close(fd[0]);
		close(fd[1]);
		wait(0);
		wait(0);
}


void set_prev_command(char *history, char *line)
{
		strcpy(history, line);
}


char *get_prev_command(char *history)
{
		if (history[0] == '\0')
		{
				fprintf(stderr, "No commands in history\n");
				return NULL;
		}
		return history;
}


int simple_shell_cd(char **args);
int simple_shell_help(char **args);
int simple_shell_exit(char **args);
void exec_command(char **args, char **redir_argv, int wait, int res);


char *builtin_str[] =
{
		"cd",
		"help",
		    "exit"
};


int (*builtin_func[])(char **) =
{
		&simple_shell_cd,
		&simple_shell_help,
		    &simple_shell_exit
};

int simple_shell_num_builtins()
{
		return sizeof(builtin_str) / sizeof(char *);
}

int simple_shell_cd(char **argv)
{
		if (argv[1] == NULL) {
				fprintf(stderr, "Error: Expected argument to \"cd\"\n");
		}
		else
		{

				if (chdir(argv[1]) != 0)
				{
						perror("Error: Error when change the process's working directory to PATH.");
				}
		}
		return 1;
}


int simple_shell_help(char **argv) {
    char *help_team_information[] = {
        "OPERATING SYSTEMS PROJECT 01 - A SIMPLE SHELL\n",
        "λ Team member λ\n",
        "18120061 \t\tNhut-Nam Le\n",
        "18120185 \t\tDang-Khoa Doan\n",
        "λ Description λ\n",
        "Khoa and Nam's Shell is a simple UNIX command interpreter that replicates functionalities of the simple shell (sh).\n",
        "This program was written entirely in C as assignment for project 01 in Operating Systems Course CQ2018-21, host by lecturers Dung Tran Trung & lab teacher Le Giang Thanh.\n",
        "\n",
        "\nUsage help command. Type help [command name] for help/ more information.\n",
        "Options for [command name]:\n",
        "cd <directory name>\t\t\tDescription: Change the current working directory.\n",
        "exit              \t\t\tDescription: Exit Khoa & Nam'shell, buyback Linux Shell.\n",
        NULL
    };
    char *help_cd_command[] = {"HELP CD COMMAND\n", NULL};
    char *help_exit_command[] = {"HELP EXIT COMMAND\n", NULL};
    int i;

    if (strcmp(argv[0], "help") == 0 && argv[1] == NULL) {
        for (i = 0; help_team_information[i] != NULL; i++) {
            printf("%s", help_team_information[i]);
        }
        return 0;
    }

    if (strcmp(argv[1], "cd") == 0) {
        for (i = 0; help_cd_command[i] != NULL; i++) {
            printf("%s", help_cd_command[i]);
        }
    } else if (strcmp(argv[1], "exit") == 0) {
        for (i = 0; help_exit_command[i] != NULL; i++) {
            printf("%s", help_exit_command[i]);
        }
    } else {
        printf("%s", "Error: Too much arguments.");
        return 1;
    }
    return 0;
}



int simple_shell_exit(char **args)
{
		int ran = 0;
	 	printf("args[0]: %s\n", args[0]);
		return ran;
}


int simple_shell_history(char *history, char **redir_args)
{
		int res;
		char *cur_args[BUFFER_SIZE];
		char cur_command[MAX_LINE_LENGTH];
		int t_wait;

		if (history[0] == '\0')
		{
				fprintf(stderr, "No commands in history\n");
				return 1;
		}
		strcpy(cur_command, history);
		printf("%s\n", cur_command);
		parse_command(cur_command, cur_args, &t_wait);
		res = 0;
		exec_command(cur_args, redir_args, t_wait, res);
		return res;
}



int simple_shell_redirect(char **args, char **redir_argv)
{
		int redir_op_index;
		redir_op_index = is_redirect(args);

		if (redir_op_index != 0)
		{
				parse_redirect(args, redir_argv, redir_op_index);
				if (strcmp(redir_argv[0], ">") == 0)
				{
						exec_child_overwrite_to_file(args, redir_argv);
				}
				else if (strcmp(redir_argv[0], "<") == 0)
				{
						exec_child_overwrite_from_file(args, redir_argv);
				}
				else if (strcmp(redir_argv[0], ">>") == 0)
				{
						exec_child_append_to_file(args, redir_argv);
				}
				return 1;
		}
		return 0;
}


int simple_shell_pipe(char **args)
{   int pipe_op_index;
		pipe_op_index = is_pipe(args);
		if (pipe_op_index != 0)
		{
				char *child01_arg[PIPE_SIZE];
				char *child02_arg[PIPE_SIZE];
				parse_pipe(args, child01_arg, child02_arg, pipe_op_index);
				exec_child_pipe(child01_arg, child02_arg);
				return 1;
		}
		return 0;
}


void exec_command(char **args, char **redir_argv, int wait, int res)
{
		int i;
		for (i = 0; i < simple_shell_num_builtins(); i++)
		{
				if (strcmp(args[0], builtin_str[i]) == 0)
				{
						(*builtin_func[i])(args);
						res = 1;
				}
		}


		if (res == 0)
		{
				int status;

				pid_t pid = fork();
				if (pid == 0)
				{

						if (res == 0) res = simple_shell_redirect(args, redir_argv);
						if (res == 0) res = simple_shell_pipe(args);
						if (res == 0) execvp(args[0], args);
						exit(EXIT_SUCCESS);

				} else if (pid < 0)
				{
						perror("Error: Error forking");
						exit(EXIT_FAILURE);
				}
				else
				{


						if (wait == 1)
						{
								waitpid(pid, &status, WUNTRACED);
						}
				}
		}
}
