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

int		mem_nxt(char *line, int *i, int j, int count)
{
	char	c;

	c = ' ';
	while (line[*i + j] && (line[*i + j] != ' ' || c != ' '))
	{
		if (c == ' ' && (line[*i + j] == '\'' || line[*i + j] == '\"'))
			c = line[*i + j++];
		else if (c != ' ' && line[*i + j] == c)
		{
			count += 2;
			c = ' ';
			j++;
		}
		else
			j++;
		if (line[*i + j - 1] == '\\')
			count--;
	}
	return (j - count + 1);
}

int		skip_del(char *str, int i)
{
	if (str[i] && str[i] == '\\' && str[i + 1] && str[i + 1] == ';')
		return (1);
	else if (str[i] && str[i] == '\\' && str[i + 1] && str[i + 1] == '|')
		return (1);
	else if (str[i] && str[i] == '\\' && str[i + 1] && str[i + 1] == '>')
		return (1);
	else if (str[i] && str[i] == '\\' && str[i + 1] && str[i + 1] == '>'
				&& str[i + 2] && str[i + 2] == '>')
		return (1);
	return (0);
}

t_token	*new_tkn(char *str, int *i, int j, char c)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	token->str = (char *)malloc(sizeof(char) * mem_nxt(str, i, 0 , 0));
	if (!(token) || !(token->str))
		return (NULL);
	while (str[*i] && (str[*i] != ' ' || c != ' '))
	{
		if (c == ' ' && (str[*i] == '\'' || str[*i] == '\"'))
			c = str[(*i)++];
		else if (c != ' ' && str[*i] == c)
		{
			c = ' ';
			(*i)++;
		}
		else if (str[*i] == '\\' && (*i)++)
			token->str[j++] = str[(*i)++];
		else
			token->str[j++] = str[(*i)++];
	}
	token->str[j] = '\0';
	return (token);
}

void	type_arg(t_token *token, int sep)
{
	if (ft_strcmp(token->str, "") == 0)
		token->type = EMPTY;
	else if (ft_strcmp(token->str, ">") == 0 && sep == 0)
		token->type = TRUNC;
	else if (ft_strcmp(token->str, ">>") == 0 && sep == 0)
		token->type = APPEND;
	else if (ft_strcmp(token->str, "<") == 0 && sep == 0)
		token->type = INPUT;
	else if (ft_strcmp(token->str, "|") == 0 && sep == 0)
		token->type = PIPE;
	else if (ft_strcmp(token->str, ";") == 0 && sep == 0)
		token->type = END;
	else if (token->prev == NULL || token->prev->type >= TRUNC)
		token->type = CMD;
	else
		token->type = ARG;
}

t_token *create_tokens(char *line)
{
	t_token	*prev;
	t_token	*next;
	int		i;
	int		sep;

	prev = NULL;
	next = NULL;
	i = 0;
	while (line[i] == ' ')
    {
        i++;
    }
	while (line[i])
	{
		sep = skip_del(line, i);
		next = new_tkn(line, &i, 0, ' ');
		next->prev = prev;
		if (prev)
			prev->next = next;
		prev = next;
		type_arg(next, sep);
		while (line[i] == ' ')
        {
            i++;
        }
	}
	if (next)
		next->next = NULL;
	while (next && next->prev)
		next = next->prev;
	return (next);
}

int		quotes(char *line, int index)
{
	int	i;
	int	open;

	i = 0;
	open = 0;
	while (line[i] && i != index) // сега
	{
		if (i > 0 && line[i - 1] == '\\')
			;
		else if (open == 0 && line[i] == '\"')
			open = 1;
		else if (open == 0 && line[i] == '\'')
			open = 2;
		else if (open == 1 && line[i] == '\"')
			open = 0;
		else if (open == 2 && line[i] == '\'')
			open = 0;
		i++;
	}
	return (open);
}

int		is_sep(char *line, int i)
{
	if (i > 0 && line[i - 1] == '\\' && ft_strchr("<>|;", line[i]))
		return (0);
	else if (ft_strchr("<>|;", line[i]) && quotes(line, i) == 0)
		return (1);
	else
		return (0);
}

char	*space_alloc(char *line)
{
	char	*new;
	int		count;
	int		i;

	count = 0;
	i = 0;
	while (line[i])
	{
		if (is_sep(line, i))
			count++;
		i++;
	}
	new = (char *)malloc(sizeof(char) * (i + 2 * count + 1));
	if (!(new))
		return (NULL);
	return (new);
}

char	*space_line(char *line)
{
	char	*new;
	int		i;
	int		j;

	i = 0;
	j = 0;
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

int		quote_check(__unused t_main *mini, char **line)
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

int magic_box(t_token *token, char **envp)
{
	char *cmd;
	char *var;

	while (token)
	{
		if(token->type == ARG || token->type == CMD)
		{
			cmd = token->str;
			if (*cmd == -36 || *cmd == 36)
			{
				free(token->str);
				if(!(var = get_envi_val(envp, cmd + 1)))
					token->str = "";
				else
					token->str = ft_strdup(var + ft_strlen(cmd));
			}
		}
		token = token->next;
	}
	return (0);
}

int point_quoter_check(char *cmd, int i)
{
	while (cmd[i])
	{
		if (cmd[i] == ';')
		{
			if (cmd[i + 1] == ';')
			{
				printf("minishell: syntax error near unexpected token \';;\'\n");
				return 1; 
			}
		}
		i++;
	}
	return 0;
}

int   parse(__unused t_main *main)
{
    int i;
    char *cmd;
	
    i = 0;
    signal(SIGINT, &sig_int);
	signal(SIGQUIT, &sig_quit);
    cmd = readline("minishell> ");
	add_history(cmd);
	if (cmd == NULL && (main->exit = 1))
	{
		ft_putstr_fd("\b\bexit\n", STDERR);
	}
	if (cmd && cmd[0])
	{
		if (quote_check(main, &cmd) || point_quoter_check(cmd, 0))
			return (0);
		cmd = space_line(cmd);
		main->token = create_tokens(cmd);
		magic_box(main->token, main->envp);
		// while(main->token)
		// {
		// 	printf("%s - %d\n", main->token->str, main->token->type);
		// 	main->token = main->token->next;
		// }
		// exit(0);
		free(cmd);
		return (1);
	}
	else
	{
		free(cmd);
		return (0);
	}
}
