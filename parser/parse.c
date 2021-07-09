/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmyesha <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 19:04:42 by dmyesha           #+#    #+#             */
/*   Updated: 2021/07/08 19:04:44 by dmyesha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*space_line(char *line, int i, int j)
{
	char	*new;

	new = space_alloc(line);
	while (new && line[i])
	{
		if (quotes(line, i) != 2 && line[i] == '$' && i && line[i - 1] != '\\')
			new[j++] = (char)(-line[i++]);
		else if (quotes(line, i) == 0 && is_sep(line, i))
		{
			new[j++] = ' ';
			new[j++] = line[i++];
			if (quotes(line, i) == 0 && line[i] == '>')
				new[j++] = line[i++];
			new[j++] = ' ';
		}
		else
			new[j++] = line[i++];
	}
	new[j] = '\0';
	free(line);
	line = NULL;
	return (new);
}

int	quote_check(char **line)
{
	if (quotes(*line, 2147483647))
	{
		ft_putendl_fd("minishell: syntax error with open quotes", STDERR);
		free(*line);
		*line = NULL;
		return (1);
	}
	return (0);
}

int	magic_box(t_token *token, char **envp)
{
	char	*cmd;
	char	*var;

	while (token)
	{
		if (token->type == ARG || token->type == CMD)
		{
			cmd = token->str;
			if ((*cmd == -36 || *cmd == 36) && *(cmd + 1) != '?')
			{
				free(token->str);
				var = get_envi_val(envp, cmd + 1);
				if (!(var))
					token->str = "";
				else
					token->str = ft_strdup(var + ft_strlen(cmd));
			}
		}
		token = token->next;
	}
	return (0);
}

int	point_quoter_check(char *cmd, int i)
{
	while (cmd[i])
	{
		if (cmd[i] == ';')
		{
			if (cmd[i + 1] == ';')
			{
				printf("minishell: syntax error near unexpected token \';;\'\n");
				return (1);
			}
		}
		i++;
	}
	return (0);
}

int	parse(t_main *main, char *cmd)
{
	signal(SIGINT, &sig_int);
	signal(SIGQUIT, &sig_quit);
	cmd = readline("minishell> ");
	add_history(cmd);
	if (cmd == NULL)
	{
		main->exit = 1;
		ft_putstr_fd("\b exit\n", STDERR);
	}
	if (cmd && cmd[0])
	{
		if (quote_check(&cmd) || point_quoter_check(cmd, 0))
			return (0);
		cmd = space_line(cmd, 0, 0);
		main->token = create_tokens(cmd, 0);
		magic_box(main->token, main->envp);
		free(cmd);
		return (1);
	}
	else
	{
		free(cmd);
		return (0);
	}
}
