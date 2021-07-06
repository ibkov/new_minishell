#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "get_next_line.h"
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <fcntl.h>
# include <dirent.h>
# include <sys/wait.h>
# include <limits.h>
# include <errno.h>
# include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

# define STDIN 0
# define STDOUT 1
# define STDERR 2

# define NOSKIP 0
# define SKIP 1

enum types
{
    EMPTY,
	CMD,
	ARG,
	TRUNC,
	APPEND,
	INPUT,
	PIPE,
	END
};


typedef struct	s_sig
{
	int				sigint;
	int				sigquit;
	int				exit_status;
	pid_t			pid;
}				t_sig;

extern t_sig g_sig;

typedef struct	s_token
{
	char			*str;
	int				type;
	struct s_token	*prev;
	struct s_token	*next;
}				t_token;

typedef struct	s_redirect
{
	int				amount;
	int				type;
	char			*redirect_file;
	int				base_fd;
}				t_redirect;

typedef struct s_main
{
    char    *base_command;
    char    **tokens;
    char    *unix_path;
    char    **envp;
    int     exit;
    int     quit;
	int		exit_code;
	t_token	*token;
	t_redirect redirect;
}                   t_main;
int    	parse(__unused t_main *main);
char    *del_spaces(char *str);
int 	argv_len(char **p);
char    *create_path(char **components, int len);
int     count_len(char **argv);
void    change_envp(char **envp, char *variable, char *value);
void    init_envp(t_main *main, char **envp);

char 	**create_argv(t_token *token);
int		search_binary(char *command, char **envp, t_main *main);
char 	*get_envi_val(char **envp, char *var);

int		is_builtin(char *command);
void execve_builtin(t_main *main);
void execve_bin(t_main *main);
int		is_bin(char *command, t_main *main);

void    sh_export(t_main *main);
void 	sh_unset(t_main *main);
void 	sh_env(t_main *main);
int 	sh_pwd(void);
int 	sh_exit(t_main *main);
void    cd(t_main *main);
void 	echo(t_main *main);
// void	exit(t_main *main);

void    free_argv(char **argv);
// void	redirect(t_redirect *redirect, char *redirect_file);
void	redirect(t_main *main);
void	sig_int(int code);
void	sig_quit(int code);
void	sig_init(void);
int 	arg_in_env(t_main *main, char *str, int j);
int 	count_env_args(t_main *main, int i);
int 	sh_echo(t_main *main);

int str_error(char *str, int ret);
#endif