#include "minishell.h"

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

int	init_envp(t_main *main, char **envp)
{
	int	i;

	i = 0;
	while (envp[i] != NULL)
		i++;
	main->envp = (char **)malloc((i + 1) * sizeof(char *));
    if(!(main->envp))
        return (1);
        // return (ALLOC_ERROR);
	main->envp[i] = NULL;
	i = 0;
	while (envp[i] != NULL)
	{
		main->envp[i] = ft_strdup(envp[i]);
		i++;
	}
	increment_lvl(main->envp);
	main->unix_path = NULL;
    return (0);
}

void	change_envp(char **envp, char *variable, char *value)
{
	int	i;

	i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], variable, ft_strlen(variable)) == 0)
		{
			free(envp[i]);
			envp[i] = NULL;
			envp[i] = ft_strjoin(variable, value);
		}
		i++;
	}
}

int	str_error(char *str, int ret)
{
	printf("%s\n", str);
	return (ret);
}