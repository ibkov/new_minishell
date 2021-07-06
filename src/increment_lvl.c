#include "minishell.h"

int increment_lvl(char **envp)
{
    char *val;
    int lvl;
    char *new_lvl;

    val = get_envi_val(envp, "SHLVL");
    while(*val++ != '=');
    lvl = ft_atoi(val);
    lvl++;
    new_lvl = ft_itoa(lvl);
    change_envp(envp, "SHLVL=", new_lvl);
    free(new_lvl);

    return (0);
}