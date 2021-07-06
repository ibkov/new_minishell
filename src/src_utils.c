#include "minishell.h"

int count_len(char **argv)
{
    int i;

    i = 0;
    while(argv[i] != NULL)
        i++;
    return (i);
}

char *create_path(char **components, int len)
{
    char *path;
    char *tmp;
    int     i;

    i = 0;
    path = "\0";
    while (components[i] != NULL && len > 0)
    {
        tmp = path;
        path = ft_strjoin(components[i], path);
        free(tmp);
        i++;
        len--;
    }
    return (path);
}

char **create_argv(t_token *token)
{
    char **arg;
    int i;
    int j;
    t_token *p;

    i = 1;
    j = 0;
    p = token;
    
    if (token->next)
       token = token->next;
    while (token->type == ARG && token->next)
    {
        i++;
        token = token->next;
    }
    if(token->type == ARG)
        i++;
    arg = (char **)malloc((i + 1) * sizeof(char *));
    while ((p->type == CMD || p->type == ARG) && p->next)
    {
        arg[j++] = p->str;
        p = p->next;
    }
    if (p->type == CMD || p->type == ARG)
    {
        arg[j++] = p->str;
    }
    arg[j] = NULL;
    return (arg);
}

char *get_envi_val(char **envp, char *var)
{
    int i;

    i = 0;
    while (envp[i] != NULL)
    {
        if (ft_strncmp(envp[i], var, ft_strlen(var)) == 0)
            return(envp[i]);
        i++;
    }
    return (NULL);
}

int str_error(char *str, int ret)
{
    printf("%s\n", str);
    return (ret);
}