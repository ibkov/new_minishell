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
    char	**temp_str1;

	while (main->declare[j])
	{
		temp_str = ft_split(main->declare[j], '=');
        temp_str1 = ft_split(str, '=');
		if ((ft_strncmp(temp_str1[0], temp_str[0], ft_strlen(temp_str[0])) == 0) \
		&& ft_strlen(temp_str[0]) == ft_strlen(temp_str1[0]))
		{
			free_argv(temp_str, 0);
			return (j);
		}
		j++;
		free_argv(temp_str, 0);
	}
	return (-1);
}

void    create_env_decl_unset(t_main *main, t_token *token, int j, int k)
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
        free_argv(main->declare, 0);
        main->declare = tmp_env;
	}
}

char *add_quoters(char *str1, char *str2)
{
    int i;
    int j;
    char *tmp;

    i = 0;
    j = 0;
    if (!str2)
        return (str1);
    tmp = (char*)malloc(sizeof(char) * (ft_strlen(str1) + ft_strlen(str2) + 4));
    while (str1[i])
    {
        tmp[i] = str1[i];
        i++;
    }
    tmp[i++] = '=';
    tmp[i++] = '\"';
    while (str2[j])
    {
        tmp[i] = str2[j];
        j++;
        i++;
    }
    tmp[i++] = '\"';
    tmp[i] = '\0';
    return (tmp);
}

void    create_env_decl_export(t_main *main, t_token *token, int j)
{
    char	**tmp_env;
	char	**temp_str;
    char	**temp_str1;
    char    *tmp;

	temp_str = NULL;
    tmp = ft_strjoin("declare -x ", token->str);
    tmp_env = (char **)malloc((count_env_args(main->declare, 0) + 2) * sizeof(char *));
    if (arg_in_decl(main, tmp, 0) != -1)
	{
        while (main->declare[j] != NULL)
        {
            temp_str = ft_split(main->declare[j], '=');
            temp_str1 = ft_split(tmp, '=');
            if (ft_strncmp(temp_str1[0], temp_str[0], ft_strlen(temp_str[0])) == 0)
            {
                free(main->declare[j]);
                main->declare[j] = add_quoters(temp_str1[0], temp_str1[1]);
                sorted(tmp_env);
                return ;   
            }
        j++;
        }
    }
    else
    {
        j = 0;
        while (main->declare[j] != NULL)
        {
            tmp_env[j] = ft_strdup(main->declare[j]);
            j++;
        }
        temp_str = ft_split(tmp, '=');
        tmp_env[j] = add_quoters(temp_str[0], temp_str[1]);
        tmp_env[j + 1] = NULL;
        sorted(tmp_env);
        free_argv(main->declare, 0);
        main->declare = NULL;
        main->declare = tmp_env;
    }
}
