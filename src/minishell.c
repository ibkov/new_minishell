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

int executor(t_main *main, t_token *token)
{
	int i;
	int **pipes;


	main->token = token;
	if ((i = is_pipe(token)) > 0)
	{
		int proc_num = i + 1;
		pipes = init_pipes(i);
		// for(int i = 0; i < proc_num - 1; i++)
		// 	pipe(pipes[i]);
		for(int i = 0; i < proc_num; i++)
		{
			if(is_bin(token->str, main))
			{
				if(i == 0)
				{
					main->tokens = create_argv(token);
					if(fork() == 0)
					{
						dup2(pipes[i][1], 1);
						for(int i = 0;i < proc_num - 1; i++)
						{
							close(pipes[i][0]);
							close(pipes[i][1]);
						}
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
				}
				else if(i == proc_num - 1)
				{
					main->tokens = create_argv(token);
					if(fork() == 0)
					{
						dup2(pipes[i - 1][0], 0);
						for(int i = 0;i < proc_num - 1; i++)
						{
							close(pipes[i][0]);
							close(pipes[i][1]);
						}
						execve(main->unix_path, main->tokens, main->envp);
					}
					else 
					{
						while (token && token->type != PIPE)
						{
							token = token->next;
						}
					}
				}
				else if(i > 0 && i < proc_num - 1)
				{
					main->tokens = create_argv(token);
					if(fork() == 0)
					{
						dup2(pipes[i - 1][0], 0);
						dup2(pipes[i][1], 1);
						for(int i = 0;i < proc_num - 1; i++)
						{
							close(pipes[i][0]);
							close(pipes[i][1]);
						}
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
				}
			}
		}
		for(int i = 0; i < proc_num - 1; i++)
		{
			close(pipes[i][0]);
			close(pipes[i][1]);
		}
		for(int i = 0; i < proc_num; i++)
		{
			wait(&g_sig.exit_status);
			WIFEXITED(g_sig.exit_status);
			g_sig.exit_status %= 255;
		}
	}
	else
	{
		if(is_builtin(token->str))
			execve_builtin(main);
		else if (is_bin(token->str, main))
			execve_bin(main);
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

int minishell(t_main *main)
{
	t_token *token;
	int i;

	token = next_cmd(main->token, NOSKIP);
	while (main->exit == 0 && token)
	{
		i = executor(main, token);
		token = next_cmd(main->token, SKIP);
	}
	return (i);
}

int main(int argc, __unused char **argv, char **envp)
{
	t_main main;
	int i;

	i = 0;
	main.exit = 0;
	if(argc == 1)
	{
		init_envp(&main, envp);
		sig_init();
		change_envp(main.envp, "SHLVL=", "2");
		while(main.exit == 0)
		{
			if (parse(&main))
			{
				while (main.token)
				{
					if (main.token && main.token->type == END)
						main.token = main.token->next;
					if(main.token && minishell(&main))
						break;
					while(main.token && main.token->type != END)
						main.token = main.token->next;
				}
				// free_argv(main.tokens);
				// free(main.base_command);
			}
		}
	}
	free_argv(main.envp);
	return (main.exit);
}