#include "minishell.h"

int heredoc(__unused t_main *main, char *delimitr)
{
	int fd[2];
	t_token *token;
	char *cmd = NULL;
	char *str = "";

	printf("%s\n", delimitr);
	pipe(fd);
	if(!fork())
	{
		close(fd[0]);
		while(1)
		{
			cmd = readline("heredoc>");
			if(ft_strcmp(cmd, delimitr) == 0)
				break;
			cmd = space_line(cmd);
			token = create_tokens(cmd);
			magic_box(token, main->envp);
			while(token)
			{
				str = ft_strjoin(str, token->str);
				token = token->next;
			}
		}
		write(fd[1], str, ft_strlen(str));
		close(fd[1]);
		exit(0);
	}
	wait(&g_sig.exit_status);
	// char buf[2];
	// buf[1] = '\0';
	// while(read(fd[0], &buf, 1) > 0)
	// {
	// 	printf("%s\n", buf);
	// }
	dup2(fd[0], 0);
	close(fd[1]);
	close(fd[0]);
	// wait(&g_sig.exit_status);
	return (0);
}

void	redirect(t_main *main)
{
	int fd;
	t_token *token;

	token = main->token;
	while (token && token->next && token->type != TRUNC && token->type != APPEND && token->type != INPUT)
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