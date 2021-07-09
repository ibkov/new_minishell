/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   declare_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmyesha <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/10 00:01:00 by dmyesha           #+#    #+#             */
/*   Updated: 2021/07/10 00:01:02 by dmyesha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	arg_in_decl(t_main *main, char *str, int j)
{
	char	**temp_str;

	while (main->declare[j])
	{
		temp_str = ft_split(main->declare[j], '=');
		if ((ft_strncmp(str, temp_str[0], ft_strlen(temp_str[0])) == 0) \
		&& ft_strlen(temp_str[0]) == ft_strlen(str))
		{
			free_argv(temp_str, 0);
			return (j);
		}
		j++;
		free_argv(temp_str, 0);
	}
	return (-1);
}

void	util(char **tmp_env, int k, t_main *main)
{
	tmp_env[k] = NULL;
	free_argv(main->declare, 0);
	main->declare = tmp_env;
}

void	create_env_declare(t_main *main, t_token *token, int j, int k)
{
	char	**tmp_env;
	char	**temp_str;
	char	*tmp;

	temp_str = NULL;
	tmp = ft_strjoin("declare -x ", token->str);
	if (arg_in_decl(main, tmp, 0) != -1)
	{
		tmp_env = (char **)
			malloc((count_env_args(main->declare, 0)) * sizeof(char *));
		while (main->declare[j] != NULL)
		{
			temp_str = ft_split(main->declare[j], '=');
			if (ft_strncmp(tmp, temp_str[0], \
			ft_strlen(temp_str[0])) != 0)
			{
				tmp_env[k] = ft_strdup(main->declare[j]);
				k++;
			}
			free_argv(temp_str, 0);
			temp_str = NULL;
			j++;
		}
		util(tmp_env, k, main);
	}
}
