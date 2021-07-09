/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmyesha <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 19:08:31 by dmyesha           #+#    #+#             */
/*   Updated: 2021/07/08 19:08:33 by dmyesha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	skip_args(t_main *main)
{
	while (main->token)
		main->token = main->token->next;
}

void	ex_digit_error(t_main *main)
{
	if (!ft_isdigit(main->token->next->str[0]))
	{
		main->exit = 1;
		main->exit_code = 255;
		printf("minishell: exit: %s: numeric argument required\n", \
		main->token->next->str);
		skip_args(main);
	}
	else
	{
		main->exit = 1;
		main->exit_code = ft_atoi(main->token->next->str) % 255;
		skip_args(main);
		printf("exit\n");
	}
}

int	sh_exit(t_main *main)
{
	if (main->token->next)
	{
		if (main->token->next->next)
		{
			printf("exit\n");
			printf("minishell: exit: too many arguments\n");
			skip_args(main);
		}
		else
			ex_digit_error(main);
	}
	else
	{
		main->exit = 1;
		printf("exit\n");
	}
	return (0);
}
