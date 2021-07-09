/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmyesha <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/09 22:42:46 by dmyesha           #+#    #+#             */
/*   Updated: 2021/07/09 22:42:48 by dmyesha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	**init_pipes(int amount_pipe)
{
	int	**pipes;
	int	i;

	i = 0;
	pipes = (int **)malloc(sizeof(int *) * (amount_pipe + 1));
	if (!pipes)
		return (NULL);
	pipes[amount_pipe] = NULL;
	while (i < amount_pipe)
	{
		pipes[i] = (int *)malloc(sizeof(int) * 2);
		if (!pipes[i])
			return (NULL);
		pipe(pipes[i]);
		i++;
	}
	return (pipes);
}

void	close_pipes(int proc_num, int **pipes)
{
	int	i;

	i = 0;
	while (i < proc_num - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

int	is_pipe(t_token *token)
{
	int	i;

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

void	wait_proccess(int proc_num)
{
	int	i;

	i = 0;
	while (i < proc_num)
	{
		wait(&g_sig.exit_status);
		g_sig.exit_status = WEXITSTATUS(g_sig.exit_status);
		g_sig.exit_status %= 256;
		i++;
	}
}
