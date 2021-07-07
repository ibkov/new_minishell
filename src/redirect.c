#include "minishell.h"

int heredoc(__unused t_main *main, char *delimitr)
{
	int fd[2];
	char *cmd = NULL;

	printf("%s", delimitr);
	pipe(fd);
	if(!fork())
	{
		dup2(fd[1], 1);
		close(fd[0]);
		close(fd[1]);
		while(cmd != delimitr)
		{
			cmd = readline("heredoc>");
			printf("%s", cmd);
		}
		exit(0);
	}
	close(fd[0]);
	close(fd[1]);
	wait(&g_sig.exit_status);
	return (0);
}

void	redirect(t_main *main)
{
	int fd;
	t_token *token;

	token = main->token;
	while (token && token->type != TRUNC && token->type != APPEND && token->type != INPUT)
	{
		token = token->next;
	}
	if (token->type == INPUT && token->next->type == INPUT)
	{
		heredoc(main, token->next->next->str);
	}
	else if(token->type == TRUNC)
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

void	pipe_redirect(t_main *main)
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
		if(!fork())
		{
			fd = open(token->next->str, O_WRONLY|O_CREAT|O_TRUNC, 0664);
			dup2(fd, 1);
			execve(main->unix_path, main->tokens, main->envp);
		}
	}
	else if(token->type == APPEND)
	{
		if(!fork())
		{
			fd = open(token->next->str, O_WRONLY|O_CREAT|O_APPEND, 0664);
			dup2(fd, 1);
			// close(fd);
			// close(fd_write);
			execve(main->unix_path, main->tokens, main->envp);
		}
	}
	else if(token->type == INPUT)
	{
		if(!fork())
		{
			fd = open(token->next->str, O_RDONLY|O_CREAT, 0664);
			dup2(fd, 0);
			execve(main->unix_path, main->tokens, main->envp);
		}
	}
	wait(0);
}