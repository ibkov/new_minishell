#include "minishell.h"

t_token *first_pipe(t_main *main, t_token *token, int **pipes, int proc_num)
{
	main->tokens = create_argv(token, 1, 0);
	if(fork() == 0)
	{
		dup2(pipes[0][1], 1);
		pipe_redirect(main, token);
		close_pipes(proc_num, pipes);
		if(is_bin(token->str, main))
			execve(main->unix_path, main->tokens, main->envp);
		else if(is_builtin(token->str))
		{
			execve_builtin(main);
			exit(0);
		}
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
		if(is_bin(token->str, main))
			execve(main->unix_path, main->tokens, main->envp);
		else if(is_builtin(token->str))
		{
			execve_builtin(main);
			exit(0);
		}
	}
	else 
	{
		while (token && token->type != PIPE)
			token = token->next;
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
		if(is_bin(token->str, main))
			execve(main->unix_path, main->tokens, main->envp);
		else if(is_builtin(token->str))
		{
			execve_builtin(main);
			exit(0);
		}
	}
	else 
	{
		while (token && token->type != PIPE)
			token = token->next;
		token = token->next;
	}
	return (token);
}
