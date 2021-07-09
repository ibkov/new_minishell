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


void sorted(char **declare)
{
    int i = 0;
    int j = 0;
    int count;

    while (declare[count])
        count++;
    while (i < count)
    {
        j = i;
        while (j < count)
        {
            if (declare[i] )
            j++;
        }
        i++;
    }
}

int	init_declare(t_main *main, char **envp)
{
	int	i;
    char *var;
    char *p;

	i = 0;
	while (envp[i] != NULL)
		i++;
	main->declare = (char **)malloc((i + 1) * sizeof(char *));
    if(!(main->declare))
        return (1);
        // return (ALLOC_ERROR);
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
	// increment_lvl(main->envp);
	main->unix_path = NULL;
    return (0);
}