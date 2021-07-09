/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmyesha <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 19:06:49 by dmyesha           #+#    #+#             */
/*   Updated: 2021/07/08 19:06:53 by dmyesha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	int	*check_space(char *str, int i, int len)
{
	int	*spaces;
	int	j;

	j = 0;
	spaces = (int *)malloc(sizeof(int) * amount_spaces(str));
	if (!(spaces))
		return (NULL);
	while (str[i])
	{
		if (str[i] == ' ')
		{
			j++;
			if (str[i + 1] != ' ')
			{
				spaces[len] = j;
				j = 0;
				len++;
			}
		}
		i++;
	}
	return (spaces);
}

static	char	*add_spaces(char *str, int spaces)
{
	char	*tmp;
	char	*new_str;

	new_str = ft_strdup(str);
	if (!(new_str))
		return (NULL);
	while (spaces > 0)
	{
		tmp = new_str;
		new_str = ft_strjoin(new_str, " ");
		free(tmp);
		spaces--;
	}
	return (new_str);
}

static	char	*pandora(t_token *token, int *spaces, char *str, int i)
{
	char	*tmp;

	while (token)
	{
		tmp = str;
		str = ft_strjoin(str, token->str);
		tmp = NULL;
		if (spaces[i])
		{
			tmp = str;
			str = add_spaces(str, spaces[i]);
			free(tmp);
			tmp = NULL;
			i++;
		}
		if (token->next == 0)
		{
			tmp = str;
			str = ft_strjoin(str, "\n");
			free(tmp);
		}
		token = token->next;
	}
	return (str);
}

static	char	*circle(char *delimitr, char **envp, char *str)
{
	char	*cmd;
	t_token	*token;
	int		*spaces;

	while (1)
	{
		cmd = readline("heredoc>");
		if (ft_strncmp(cmd, delimitr, ft_strlen(delimitr)) == 0)
			break ;
		spaces = check_space(cmd, 0, 0);
		cmd = space_line(cmd, 0, 0);
		token = create_tokens(cmd, 0);
		magic_box(token, envp);
		str = pandora(token, spaces, str, 0);
		free(spaces);
		spaces = NULL;
	}
	return (str);
}

void	heredoc(char **envp, char *delimitr)
{
	int		fd[2];
	char	*str;

	pipe(fd);
	if (!fork())
	{
		close(fd[0]);
		str = circle(delimitr, envp, "");
		write(fd[1], str, ft_strlen(str));
		close(fd[1]);
		exit(0);
	}
	wait(&g_sig.exit_status);
	g_sig.exit_status = WEXITSTATUS(g_sig.exit_status);
	dup2(fd[0], 0);
	close(fd[1]);
	close(fd[0]);
}
