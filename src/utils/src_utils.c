/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   src_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmyesha <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 19:06:41 by dmyesha           #+#    #+#             */
/*   Updated: 2021/07/08 19:06:43 by dmyesha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_len(char **argv)
{
	int	i;

	i = 0;
	while (argv[i] != NULL)
		i++;
	return (i);
}

char	*create_path(char **components, int len)
{
	char	*path;
	char	*tmp;
	int		i;

	i = 0;
	path = "\0";
	while (components[i] != NULL && len > 0)
	{
		tmp = path;
		path = ft_strjoin(components[i], path);
		free(tmp);
		i++;
		len--;
	}
	return (path);
}

static int	skip_arg(t_token *token, int i)
{
	if (token->next)
		token = token->next;
	while (token->type == ARG && token->next)
	{
		i++;
		token = token->next;
	}
	if (token->type == ARG)
		i++;
	return (i);
}

char	**create_argv(t_token *token, int i, int j)
{
	char	**arg;
	t_token	*p;

	p = token;
	i = skip_arg(token, i);
	arg = (char **)malloc((i + 1) * sizeof(char *));
	while ((p->type == CMD || p->type == ARG) && p->next)
	{
		arg[j++] = p->str;
		p = p->next;
	}
	if (p->type == CMD || p->type == ARG)
	{
		arg[j++] = p->str;
	}
	arg[j] = NULL;
	return (arg);
}

char	*get_envi_val(char **envp, char *var)
{
	int	i;

	i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], var, ft_strlen(var)) == 0)
			return (envp[i]);
		i++;
	}
	return (NULL);
}
