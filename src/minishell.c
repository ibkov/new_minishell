#include <stdio.h>
#include "minishell.h"

t_sig	g_sig;

int is_pipe(t_token *token)
{
	int i;

	i = 0;
	while (token && token->type != END)
	{
		if (token->type == PIPE)
		{
			i++;
		}
		token = token->next;
	}
	return (i);
}

int **init_pipes(int amount_pipe)
{
	int **pipes;
	int i;

	i = 0;
	pipes = (int **)malloc(sizeof(int *) * (amount_pipe + 1));
	if(!pipes)
		return (NULL);
	pipes[amount_pipe] = NULL;
	while(i < amount_pipe)
	{
		pipes[i] = (int *)malloc(sizeof(int) * 2);
		if(!pipes[i])
			return (NULL);
		pipe(pipes[i]);
		i++;
	}
	return (pipes);
}

void close_pipes(int proc_num, int **pipes)
{
	int i;

	i = 0;
	while(i < proc_num - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

void wait_proccess(int proc_num)
{
	int i;

	i = 0;
	while(i < proc_num)
	{
		wait(&g_sig.exit_status);
		WIFEXITED(g_sig.exit_status);
		g_sig.exit_status %= 255;
		i++;
	}
}

t_token *first_pipe(t_main *main, t_token *token, int **pipes, int proc_num)
{
	main->tokens = create_argv(token, 1, 0);
	if(fork() == 0)
	{
		dup2(pipes[0][1], 1);
		pipe_redirect(main, token);
		close_pipes(proc_num, pipes);
		if(is_builtin(token->str))
		{
			execve_builtin(main);
			exit(0);
		}
		else if(is_bin(token->str, main))
			execve(main->unix_path, main->tokens, main->envp);
	}
	else 
	{
		while (token && token->type != PIPE)
			token = token->next;
		token = token->next;
	}
	return (token);
}

t_token *last_pipe(t_main *main, t_token *token, int **pipes, int proc_num, int i)
{
	main->tokens = create_argv(token, 1, 0);
	if(fork() == 0)
	{
		dup2(pipes[i - 1][0], 0);
		close_pipes(proc_num, pipes);
		pipe_redirect(main, token);
		if(is_builtin(token->str))
		{
			execve_builtin(main);
			exit(0);
		}
		else if(is_bin(token->str, main))
			execve(main->unix_path, main->tokens, main->envp);
	}
	else 
	{
		while (token && token->type != PIPE)
		{
			token = token->next;
		}
	}
	return (token);
}

t_token *middle_pipe(t_main *main, t_token *token, int **pipes, int proc_num, int i)
{
	main->tokens = create_argv(token, 1, 0);
	if(fork() == 0)
	{
		dup2(pipes[i - 1][0], 0);
		dup2(pipes[i][1], 1);
		close_pipes(proc_num, pipes);
		pipe_redirect(main, token);
		if(is_builtin(token->str))
		{
			execve_builtin(main);
			exit(0);
		}
		else if(is_bin(token->str, main))
			execve(main->unix_path, main->tokens, main->envp);
	}
	else 
	{
		while (token && token->type != PIPE)
		{
			token = token->next;
		}
		token = token->next;
	}
	return (token);
}

void free_arg(t_main *main)
{
	if(main->tokens)
	{
		free(main->tokens);
		main->tokens = NULL;
	}
	free(main->unix_path);
	main->unix_path = NULL;
}

int executor(t_main *main, t_token *token)
{
	int i;
	int **pipes;

	main->token = token;
	if ((i = is_pipe(token)) > 0)
	{
		int proc_num = i + 1;
		pipes = init_pipes(i);
		for(int i = 0; i < proc_num; i++)
		{
			if(i == 0)
			{
				token = first_pipe(main, token, pipes, proc_num);
			}
			else if(i == proc_num - 1)
			{
				token = last_pipe(main, token, pipes, proc_num, i);
			}
			else if(i > 0 && i < proc_num - 1)
			{
				token = middle_pipe(main, token, pipes, proc_num, i);
			}
			free_arg(main);
		}
		close_pipes(proc_num, pipes);
		wait_proccess(proc_num);
		free_int(pipes);
	}
	else
	{
		if(is_builtin(token->str))
			execve_builtin(main);
		else if (is_bin(token->str, main))
			execve_bin(main);
		else
			printf("minishell: %s: command not found\n", token->str);
		free_arg(main);
	}
	return (0);
}

t_token	*next_cmd(t_token *token, int skip)
{
	if (token && skip)
		token = token->next;
	while (token && token->type != CMD)
	{
		token = token->next;
		if (token && token->type == CMD && token->prev == NULL)
			;
		else if (token && token->type == CMD && token->prev->type < END)
			token = token->next;
	}
	return (token);
}

int minishell(t_main *main, t_token *main_token)
{
	t_token *token;
	int i;

	token = next_cmd(main_token, 0);
	while (main->exit == 0 && token)
	{
		i = executor(main, token);
		token = next_cmd(main_token, 1);
	}
	return (i);
}

static void all_freed(t_main *main)
{
	int i;

	i = 0;
	if(main->token)
	{
		while(main->token)
		{
			free(main->token->str);
			if(main->token->next)
			{	
				main->token = main->token->next;
				free(main->token->prev);
			}
			else
				free(main->token);
		}
	}
}

t_token *next_token(t_token *token)
{
	if(!(token->next))
	{
		free(token->str);
		free(token);
		return (NULL);
	}
	else
	{	
		while(token && token->next && token->type != END)
		{
			token = token->next;
			if(token)
			{
				free(token->prev->str);
				free(token->prev);
			}
		}
	}
	if(token && !(token->next))
	{
		free(token->str);
		free(token);
		return (NULL);
	}
	return (token);
}

void init(t_main *main, char **envp)
{
	init_envp(main, envp);
	sig_init();
	main->tokens = NULL;
	main->token = NULL;
}

int main(int argc, __unused char **argv, char **envp)
{
	t_main main;
	
	int i;

	i = 0;
	main.exit = 0;
	if(argc == 1)
	{
		init(&main, envp);
		while(main.exit == 0)
		{
			if (parse(&main))
			{
				while (main.token)
				{
					if (main.token && main.token->type == END)
						main.token = main.token->next;
					if(main.token && minishell(&main, main.token))
						break;
					main.token = next_token(main.token);
				}
			}
			all_freed(&main);
		}
		free_argv(main.envp);
	}
	return (main.exit_code);
}