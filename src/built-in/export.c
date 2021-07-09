/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmyesha <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 19:09:17 by dmyesha           #+#    #+#             */
/*   Updated: 2021/07/08 19:09:21 by dmyesha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_env_part(t_main *main, t_token *token, int i)
{
	char	**tmp_env;

	// printf("%s", token->str);
	i = count_env_args(main->envp, 0);
	tmp_env = (char **)malloc((i + 2) * sizeof(char *));
	tmp_env[i + 1] = NULL;
	i = 0;
	while (main->envp[i] != NULL)
	{
		tmp_env[i] = ft_strdup(main->envp[i]);
		i++;
	}
	tmp_env[i] = ft_strdup(main->envp[i - 1]);
	tmp_env[i - 1] = ft_strdup(token->str);
	main->envp = tmp_env;
	// free_argv(tmp_env);
}

void add_to_declare(t_main *main, char *str)
{
	char	**tmp_env;
	int i;

	i = count_env_args(main->declare, 0);
	tmp_env = (char **)malloc((i + 2) * sizeof(char *));
	tmp_env[i + 1] = NULL;
	i = 0;
	while (main->declare[i] != NULL)
	{
		tmp_env[i] = ft_strdup(main->declare[i]);
		i++;
	}
	tmp_env[i] = ft_strjoin("declare -x ", str);
	free_argv(main->declare);
	main->declare = NULL;
	main->declare = tmp_env;
}

void	create_env_ex(t_main *main, t_token *token)
{
	char	**str;
	int		line;

	str = ft_split(token->str, '=');
	line = arg_in_env(main, str[0], 0);
	if (line != -1)
	{
		main->envp[line] = (char *)malloc(sizeof(char) * \
		(ft_strlen(token->str) + 1));
		ft_strlcpy(main->envp[line], token->str, \
		ft_strlen(token->str) + 1);
		// free(str);
	}
	else if (ft_strchr(token->str, '='))
		create_env_part(main, token, 0);
	else
		add_to_declare(main, token->str);
	free(str);
}

void	sh_export(t_main *main)
{
	t_token *token;

	token = main->token;
	if (token && (!token->next \
	|| (token->next && token->next->type != ARG)))
	{
		for(int i = 0; main->declare[i] != NULL; i++)
			printf("%s\n", main->declare[i]);
	}
	else
	{
		while ((token && token->type != END \
		&& token->type != PIPE))
		{
			if (ft_isdigit(token->str[0]))
			{
				printf("export: not an identifier: %s\n", token->str);
				return ;
			}
			else if (token->type == ARG)
			{
				init_declare(main, main->envp);
				create_env_ex(main, token);
			}
			token = token->next;
		}
	}
}
