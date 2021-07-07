#include "minishell.h"

int *check_space(char *str)
{
	int *spaces;
	int i = 1;
	int j = 0;

	while (str[i] != '\0')
	{
		if(str[i] == ' ' && str[i - 1] != ' ')
			j++;
		i++;
	}
	i = 0;
	spaces = (int *)malloc(sizeof(int ) * j);
	j = 0;
	int len = 0;
	while (str[i])
	{
		if(str[i] == ' ')
		{
			j++;
			if(str[i + 1] != ' ')
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

char *add_spaces(char *str, int spaces)
{
	char *tmp;
	char *new_str;

	new_str = ft_strdup(str);
	while(spaces > 0)
	{
		tmp = new_str;
		new_str = ft_strjoin(new_str, " ");
		free(tmp);
		spaces--;
	}
	return (new_str);
}

void heredoc(__unused t_main *main, __unused char *delimitr)
{
	int fd[2];
	t_token *token;
	char *cmd = NULL;
	char *str = "";
	int *spaces;
	int i = 0;

	pipe(fd);
	if(!fork())
	{
		close(fd[0]);
		while(1)
		{
			cmd = readline("heredoc>");
			if(ft_strncmp(cmd, delimitr, ft_strlen(delimitr)) == 0)
				break;
			spaces = check_space(cmd);
			cmd = space_line(cmd);
			token = create_tokens(cmd);
			magic_box(token, main->envp);
			while(token)
			{
				str = ft_strjoin(str, token->str);
				str = add_spaces(str, spaces[i]);
				i++;
				if (token->next == 0)
					str = ft_strjoin(str, "\n");
				token = token->next;
			}
		}
		write(fd[1], str, ft_strlen(str));
		close(fd[1]);
		exit(0);
	}
	wait(&g_sig.exit_status);
	dup2(fd[0], 0);
	close(fd[1]);
	close(fd[0]);
}

void	redirect(t_main *main)
{
	int fd;
	t_token *token;
	int type;
	char *redirect_file;

	token = main->token;
	while (token && token->next && token->type != TRUNC && token->type != APPEND && token->type != INPUT)
	{
		token = token->next;
	}
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
		{
			open(redirect_file, O_WRONLY|O_CREAT|O_TRUNC, 0664);
		}
		token = token->next;
	}
	if (type == 11)
	{
		heredoc(main, redirect_file);
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

void	pipe_redirect(t_main *main, t_token *tokens)
{
	int fd;
	t_token *token;
	int type;
	char *redirect_file;

	token = tokens;
	while (token->type != PIPE && token->type != TRUNC && token->type != APPEND && token->next && token->type != INPUT)
	{
		token = token->next;
	}
	if (token->type == PIPE)
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
	if(type == TRUNC)
	{	
		if(!fork())
		{
			fd = open(redirect_file, O_WRONLY|O_CREAT|O_TRUNC, 0664);
			dup2(fd, 1);
			execve(main->unix_path, main->tokens, main->envp);
		}
	}
	else if(type == APPEND)
	{
		if(!fork())
		{
			fd = open(redirect_file, O_WRONLY|O_CREAT|O_APPEND, 0664);
			dup2(fd, 1);
			execve(main->unix_path, main->tokens, main->envp);
		}
	}
	else if(type == INPUT)
	{
		if(!fork())
		{
			fd = open(redirect_file, O_RDONLY|O_CREAT, 0664);
			dup2(fd, 0);
			execve(main->unix_path, main->tokens, main->envp);
		}
	}
	wait(0);
}