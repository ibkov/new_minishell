/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmyesha <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 19:08:21 by dmyesha           #+#    #+#             */
/*   Updated: 2021/07/08 19:08:24 by dmyesha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	show_declare(t_main *main)
{
	int		i;
	char	*tmp;

	i = 0;
	while (main->envp[i])
	{
		tmp = main->envp[i];
		ft_putstr_fd("declare -x ", 1);
		while (*tmp != '=')
		{
			ft_putchar_fd(*tmp, 1);
			tmp++;
		}
		ft_putchar_fd(*tmp, 1);
		ft_putchar_fd('\"', 1);
		tmp++;
		ft_putstr_fd(tmp, 1);
		ft_putstr_fd("\"\n", 1);
		i++;
	}
}

void	sh_env(t_main *main, char *str)
{
	char	**envp;
	int		i;

	if (ft_strcmp(str, "export") == 0)
		show_declare(main);
	else
	{
		envp = main->envp;
		i = 0;
		while (envp[i] != NULL)
		{
			printf("%s\n", envp[i++]);
		}
	}
}
