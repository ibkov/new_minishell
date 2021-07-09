#include "minishell.h"

static char *add_quotes(char *str)
{
    int new_size;
    char **tmp;
    char *p;

    new_size = ft_strlen(str) + 2;
    tmp = ft_split(str, '=');
    p = tmp[0];
    tmp[0] = ft_strjoin(tmp[0], "=");
    free(p);
    p = NULL;
    p = tmp[1];
    tmp[1] = ft_strjoin("\"", tmp[1]);
    free(p);
    p = NULL;
    p = tmp[1];
    tmp[1] = ft_strjoin(tmp[1], "\"");
    free(p);
    free(str);
    p = ft_strjoin(tmp[0], tmp[1]);
    free_argv(tmp);
    return(p);
}


char **sorted(char **declare)
{
    int i = 0;
    int j = 0;
    int count = 0;
    char *tmp;

    while (declare[count])
        count++;
    while (i < count)
    {
        j = 0;
        while (j < count - 1)
        {
            if (ft_strcmp(declare[j], declare[j + 1]) > 0)
            {
                tmp = declare[j];
                declare[j] = declare[j + 1];
                declare[j + 1] = tmp;
            }
            j++;
        }
        i++;
    }
    return (declare);
}

int	init_declare(t_main *main, char **envp)
{
	int	i;
    char *var;
    char *p;

	i = 0;

    if (main->declare)
    {
        free_argv(main->declare);
        main->declare = NULL;
    }
	while (envp[i] != NULL)
		i++;
	main->declare = (char **)malloc((i + 1) * sizeof(char *));
    if(!(main->declare))
        return (1);
	main->declare[i] = NULL;
	i = 0;
	while (envp[i] != NULL)
	{
        var = add_quotes(ft_strdup(envp[i]));
        p = var;
        var = ft_strjoin("declare -x ", var);
        free(p);
        p = NULL;
		main->declare[i] = var;
		i++;
	}
    sorted(main->declare);
    return (0);
}

