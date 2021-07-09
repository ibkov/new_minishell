/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmyesha <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 19:09:35 by dmyesha           #+#    #+#             */
/*   Updated: 2021/07/08 19:09:40 by dmyesha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	arg_in_env(t_main *main, char *str, int j)
{
	char	**temp_str;

	while (main->envp[j])
	{
		temp_str = ft_split(main->envp[j], '=');
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

int	count_env_args(char **envp, int i)
{
	while (envp[i])
	{
		i++;
	}
	return (i);
}

void	create_env(t_main *main, t_token *token, int j, int k)
{
	char	**tmp_env;
	char	**temp_str;

	temp_str = NULL;
	if (arg_in_env(main, token->str, 0) != -1)
	{
		tmp_env = (char **)malloc((count_env_args \
			(main->envp, 0)) * sizeof(char *));
		while (main->envp[j] != NULL)
		{
			temp_str = ft_split(main->envp[j], '=');
			if (ft_strncmp(token->str, temp_str[0], \
			ft_strlen(temp_str[0])) != 0)
			{
				tmp_env[k] = ft_strdup(main->envp[j]);
				k++;
			}
			free_argv(temp_str, 0);
			temp_str = NULL;
			j++;
		}
		tmp_env[k] = NULL;
		free_argv(main->envp, 0);
		main->envp = tmp_env;
	}
}

void	sh_unset(t_main *main)
{
	int		i;
	t_token	*token;

	token = main->token;
	i = 1;
	if (!token->next)
		printf("%s", "unset: not enough arguments\n");
	else
	{
		while (token && token->type != END)
		{
			if (ft_isdigit(token->str[0]))
			{
				printf("unset: %s: invalid parameter name\n", token->str);
				return ;
			}
			else if (token->type == ARG)
				create_env(main, token, 0, 0);
			token = token->next;
		}
	}
}
