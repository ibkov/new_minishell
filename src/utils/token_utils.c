#include "minishell.h"

t_token	*next_cmd(t_token *token, int skip)
{
	if (token && skip)
		token = token->next;
	while (token && token->type != CMD)
	{
		token = token->next;
		if (token && token->type == CMD && token->prev == NULL)
			;
		else if (token && token->type == CMD && token->prev->type < END)
			token = token->next;
	}
	return (token);
}

t_token *next_token(t_token *token)
{
	if(!(token->next))
	{
		free(token->str);
		free(token);
		return (NULL);
	}
	else
	{	
		while(token && token->next && token->type != END)
		{
			token = token->next;
			if(token)
			{
				free(token->prev->str);
				free(token->prev);
			}
		}
	}
	if(token && !(token->next))
	{
		free(token->str);
		free(token);
		return (NULL);
	}
	return (token);
}