/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmyesha <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 19:07:02 by dmyesha           #+#    #+#             */
/*   Updated: 2021/07/08 19:07:17 by dmyesha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_sig	g_sig;

int executor(t_main *main, t_token *token)
{
	int i;
	int **pipes;
	int j;

	j = 0;
	main->token = token;
	if ((i = is_pipe(token)) > 0)
	{
		int proc_num = i + 1;
		pipes = init_pipes(i);
		while(j < proc_num)
		{
			if(j == 0)
				token = first_pipe(main, token, pipes, proc_num);
			else if(j == proc_num - 1)
				token = last_pipe(main, token, pipes, proc_num, j);
			else if(j > 0 && j < proc_num - 1)
				token = middle_pipe(main, token, pipes, proc_num, j);
			free_arg(main);
			j++;
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

int minishell(t_main *main, t_token *main_token)
{
	t_token	*token;
	int		i;

	token = next_cmd(main_token, 0);
	while (main->exit == 0 && token)
	{
		i = executor(main, token);
		token = next_cmd(token, 1);
	}
	return (i);
}

void init(t_main *main, char **envp)
{
	init_envp(main, envp);
	main->declare = NULL;
	init_declare(main, main->envp);
	sig_init();
	main->tokens = NULL;
	main->token = NULL;
	main->main_read = dup(0);
	main->main_write = dup(1);
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
