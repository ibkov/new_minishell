/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freed.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmyesha <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/09 22:42:08 by dmyesha           #+#    #+#             */
/*   Updated: 2021/07/09 22:42:11 by dmyesha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	all_freed(t_main *main)
{
	int	i;

	i = 0;
	if (main->token)
	{
		while (main->token)
		{
			free(main->token->str);
			if (main->token->next)
			{	
				main->token = main->token->next;
				free(main->token->prev);
			}
			else
			{
				free(main->token);
				main->token = NULL;
			}
		}
	}
}

void	free_arg(t_main *main)
{
	if (main->tokens)
	{
		free(main->tokens);
		main->tokens = NULL;
	}
	free(main->unix_path);
	main->unix_path = NULL;
}

int	free_argv(char **argv, int ret)
{
	int	i;

	i = 0;
	while (argv[i] != NULL)
		i++;
	while (i >= 0)
	{
		free(argv[i]);
		i--;
	}
	free(argv);
	argv = NULL;
	return (ret);
}

void	free_int(int **argv)
{
	int	i;

	i = 0;
	while (argv[i] != NULL)
		i++;
	while (i >= 0)
	{
		free(argv[i]);
		i--;
	}
	free(argv);
	argv = NULL;
}

int	amount_spaces(char *str)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == ' ' && str[i + 1] != ' ')
			count++;
		i++;
	}
	return (count);
}
