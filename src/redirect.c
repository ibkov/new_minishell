#include "minishell.h"

void	redirect(t_main *main)
{
	int fd;
	t_token *token;

	token = main->token;
	while (token->type != TRUNC && token->type != APPEND && token->next && token->type != INPUT)
	{
		token = token->next;
	}
	if(token->type == TRUNC)
	{
		fd = open(token->next->str, O_WRONLY|O_CREAT|O_TRUNC, 0664);
		dup2(fd, 1);
	}
	else if(token->type == APPEND)
	{
		fd = open(token->next->str, O_WRONLY|O_CREAT|O_APPEND, 0664);
		dup2(fd, 1);
	}
	else if(token->type == INPUT)
	{
		fd = open(token->next->str, O_RDONLY|O_CREAT, 0664);
		dup2(fd, 0);
	}
}