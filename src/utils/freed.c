#include "minishell.h"

void all_freed(t_main *main)
{
	int i;

	i = 0;
	if(main->token)
	{
		while(main->token)
		{
			free(main->token->str);
			if(main->token->next)
			{	
				main->token = main->token->next;
				free(main->token->prev);
			}
			else
			{
				free(main->token);
				main->token = NULL;
			}
		}
	}
}

void free_arg(t_main *main)
{
	if(main->tokens)
	{
		free(main->tokens);
		main->tokens = NULL;
	}
	free(main->unix_path);
	main->unix_path = NULL;
}

void	free_argv(char **argv)
{
	int	i;

	i = 0;
	while (argv[i] != NULL)
		i++;
	while (i >= 0)
	{
		free(argv[i]);
		i--;
	}
	free(argv);
}

void	free_int(int **argv)
{
	int	i;

	i = 0;
	while (argv[i] != NULL)
		i++;
	while (i >= 0)
	{
		free(argv[i]);
		i--;
	}
	free(argv);
}