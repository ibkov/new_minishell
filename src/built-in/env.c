#include "minishell.h"

void sh_env(t_main *main)
{
    char **envp;
    int i;

    envp = main->envp;
    i = 0;
    redirect(main);
    while (envp[i] != NULL)
	{
		printf("%s\n", envp[i++]);
	}
    dup2(0, 1);
}