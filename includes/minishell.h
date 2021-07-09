/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmyesha <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 19:04:22 by dmyesha           #+#    #+#             */
/*   Updated: 2021/07/08 19:04:24 by dmyesha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
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
# include <readline/readline.h>
# include <readline/history.h>

# define STDIN 0
# define STDOUT 1
# define STDERR 2

# define NOSKIP 0
# define SKIP 1

# define STR_ERROR 12
# define STR_NOFILE 11
# define STD_ERROR 10

enum e_types
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

typedef struct s_sig
{
	int				sigint;
	int				sigquit;
	int				exit_status;
	pid_t			pid;
}				t_sig;

extern t_sig	g_sig;

typedef struct s_token
{
	char			*str;
	int				type;
	struct s_token	*prev;
	struct s_token	*next;
}				t_token;

typedef struct s_redirect
{
	int				amount;
	int				type;
	char			*redirect_file;
	int				base_fd;
}				t_redirect;

typedef struct s_main
{
	char			*base_command;
	int				main_write;
	int				main_read;
	char			**tokens;
	char			*unix_path;
	char			**envp;
	char			**declare;
	int				exit;
	int				quit;
	int				error;
	int				exit_code;
	int				**pipes;
	t_token			*token;
	t_redirect		redirect;
}				t_main;
int					parse(t_main *main, char *cmd);
char				*del_spaces(char *str);
int					argv_len(char **p);
char				*create_path(char **components, int len);
void				change_envp(char **envp, char *variable, char *value);
int					init_envp(t_main *main, char **envp);
char				*space_alloc(char *line);
int					quotes(char *line, int index);
int					is_sep(char *line, int i);

char				*space_line(char *line, int i, int j);
t_token				*create_tokens(char *line, int i);
int					magic_box(t_token *token, char **envp);
char				**sorted(char **declare);

int					is_builtin(char *command);
void				execve_builtin(t_main *main);
void				execve_bin(t_main *main);
int					is_bin(char *command, t_main *main);
//Pipes
t_token				*middle_pipe(t_main *main, t_token *token,
						int proc_num, int i);
t_token				*last_pipe(t_main *main, t_token *token,
						int proc_num, int i);
t_token				*first_pipe(t_main *main, t_token *token,
						int proc_num);
int					**init_pipes(int amount_pipe);
void				close_pipes(int proc_num, int **pipes);
int					is_pipe(t_token *token);
void				wait_proccess(int proc_num);
//Utils
//free utils
void				all_freed(t_main *main);
void				free_arg(t_main *main);
int					free_argv(char **argv, int ret);
void				free_int(int **argv);
int					increment_lvl(char **envp);
//search file utils
int					search_binary(char *command, char **envp, t_main *main);
char				*search_file(DIR *dir, char *file_path, char *command);
int					search_in_path(char **envp, char *command,
						t_main *main, int i);
int					search_in_command(char *command, t_main *main);
int					search_in_current_dir(char **envp,
						char *command, t_main *main);
//src utils
char				*get_envi_val(char **envp, char *var);
char				**create_argv(t_token *token, int i, int j);
char				*create_path(char **components, int len);
int					count_len(char **argv);
//token_utils
t_token				*next_cmd(t_token *token, int skip);
t_token				*next_token(t_token *token);

void				sh_export(t_main *main);
void				sh_unset(t_main *main);
void				sh_env(t_main *main, char *str);
int					sh_pwd(void);
int					sh_exit(t_main *main);
void				cd(t_main *main);
void				echo(t_main *main);
void				sh_echo(t_main *main, int i);
// void	exit(t_main *main);

void				free_int(int **argv);
int					amount_spaces(char *str);
// void	redirect(t_redirect *redirect, char *redirect_file);
void				redirect(t_main *main);
void				pipe_redirect(t_main *main, t_token *tokens);
void				heredoc(char **envp, char *delimitr);
void				sig_int(int code);
void				sig_quit(int code);
void				sig_init(void);
int					arg_in_env(t_main *main, char *str, int j);
int					count_env_args(char **envp, int i);
int					init_declare(t_main *main, char **envp);
int					str_error(char *str, int ret);

#endif
