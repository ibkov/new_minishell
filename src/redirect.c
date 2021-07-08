#include "minishell.h"

static void execure_redirect(int type, char *redirect_file, char **envp)
{
	int fd;

	if (type == 11)
	{
		heredoc(envp, redirect_file);
	}
	if(type == TRUNC)
	{
		fd = open(redirect_file, O_WRONLY|O_CREAT|O_TRUNC, 0664);
		dup2(fd, 1);
	}
	else if(type == APPEND)
	{
		fd = open(redirect_file, O_WRONLY|O_CREAT|O_APPEND, 0664);
		dup2(fd, 1);
	}
	else if(type == INPUT)
	{
		fd = open(redirect_file, O_RDONLY|O_CREAT, 0664);
		dup2(fd, 0);
	}
}

void	redirect(t_main *main)
{
	int fd;
	t_token *token;
	int type;
	char *redirect_file;

	token = main->token;
	while (token && token->next && token->type != TRUNC 
	&& token->type != APPEND && token->type != INPUT)
		token = token->next;
	if (token->type == PIPE || token->type == CMD)
		return ;
	if(token->type == INPUT && token->next->type == INPUT)
		type = 11;
	else
		type = token->type;
	token = token->next;
	while(token)
	{
		if(token->type < 3)
			redirect_file = token->str;
		else if (token->type == type)
			open(redirect_file, O_WRONLY|O_CREAT|O_TRUNC, 0664);
		token = token->next;
	}
	execure_redirect(type, redirect_file, main->envp);
}

static void execute_pipe_redirect(int type, char *redirect_file, t_main *main)
{
	int fd;

	if(!fork())
	{
		if(type == TRUNC)
		{
			fd = open(redirect_file, O_WRONLY|O_CREAT|O_TRUNC, 0664);
			dup2(fd, 1);
			execve(main->unix_path, main->tokens, main->envp);
		}
		else if (type == APPEND)
		{
			fd = open(redirect_file, O_WRONLY|O_CREAT|O_APPEND, 0664);
			dup2(fd, 1);
			execve(main->unix_path, main->tokens, main->envp);
		}
		else if (type == INPUT)
		{
			fd = open(redirect_file, O_RDONLY|O_CREAT, 0664);
			dup2(fd, 0);
			execve(main->unix_path, main->tokens, main->envp);
		}
	}
	wait(&g_sig.exit_status);
}

void	pipe_redirect(__unused t_main *main, t_token *tokens)
{
	t_token *token;
	int type;
	char *redirect_file;

	token = tokens;
	while (token->type != PIPE && token->type != TRUNC && token->type != APPEND && token->next && token->type != INPUT)
	{
		token = token->next;
	}
	if (token->type == PIPE || token->type == CMD)
		return ;
	if(token->type == INPUT && token->next->type == INPUT)
		type = 11;
	else
		type = token->type;
	token = token->next;
	while(token)
	{
		if (token->type == PIPE)
			break;
		if(token->type < 3)
			redirect_file = token->str;
		else if (token->type == type)
		{
			open(redirect_file, O_WRONLY|O_CREAT|O_TRUNC, 0664);
		}
		token = token->next;
	}
	execute_pipe_redirect(type, redirect_file, main);
}