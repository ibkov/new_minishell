#include "minishell.h"


int	arg_in_decl(t_main *main, char *str, int j)
{
	char	**temp_str;

	while (main->declare[j])
	{
		temp_str = ft_split(main->declare[j], '=');
		if ((ft_strncmp(str, temp_str[0], ft_strlen(temp_str[0])) == 0) \
		&& ft_strlen(temp_str[0]) == ft_strlen(str))
		{
			free_argv(temp_str);
			return (j);
		}
		j++;
		free_argv(temp_str);
	}
	return (-1);
}

void    create_env_declare(t_main *main, t_token *token, int j, int k)
{
	char	**tmp_env;
	char	**temp_str;
    char    *tmp;

	temp_str = NULL;
    tmp = ft_strjoin("declare -x ", token->str);
	if (arg_in_decl(main, tmp, 0) != -1)
	{
		tmp_env = (char **)malloc((count_env_args(main->declare, 0)) * sizeof(char *));
		while (main->declare[j] != NULL)
		{
			temp_str = ft_split(main->declare[j], '=');
			if (ft_strncmp(tmp, temp_str[0], \
			ft_strlen(temp_str[0])) != 0)
			{
				tmp_env[k] = ft_strdup(main->declare[j]);
				k++;
			}
			free_argv(temp_str);
			temp_str = NULL;
			j++;
		}
		tmp_env[k] = NULL;
		free_argv(main->declare);
		main->declare = tmp_env;
	}
}