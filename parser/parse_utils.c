#include "minishell.h"

int	mem_nxt(char *line, int *i, int j, int count)
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

int	skip_del(char *str, int i)
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
	token->str = (char *)malloc(sizeof(char) * mem_nxt(str, i, 0, 0));
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

t_token	*create_tokens(char *line, int i)
{
	t_token	*prev;
	t_token	*next;
	int		sep;

	prev = NULL;
	next = NULL;
	while (line[i] == ' ')
		i++;
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
			i++;
	}
	if (next)
		next->next = NULL;
	while (next && next->prev)
		next = next->prev;
	return (next);
}
