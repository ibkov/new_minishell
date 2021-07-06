#include "minishell.h"

void skip_args(t_main *main)
{
    while (main->token)
        main->token = main->token->next;
}

int sh_exit(t_main *main)
{
    if (main->token->next)
    {
        if (main->token->next->next)
        {
            printf("exit\n");
            printf("minishell: exit: too many arguments\n");
            skip_args(main);
        }
        else if (!ft_isdigit(main->token->next->str[0]))
        {
            main->exit = 255;
            printf("minishell: exit: %s: numeric argument required\n", main->token->next->str);
            skip_args(main);
        }
        else
        {
            main->exit = ft_atoi(main->token->next->str) % 256;
            skip_args(main);
            printf("exit\n");
        }    
    }
    else
    {
        main->exit = 1;
        printf("exit\n");
    }
    return (0);
}