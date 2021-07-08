#include "minishell.h"

void	skip_one_tkn(t_main *main, int *n_flag)
{
	*n_flag = 1;
	main->token = main->token->next;
	main->token = main->token->next;
}

int	sh_echo(t_main *main)
{
	int	n_flag;

	n_flag = 0;
	if (!main->token->next)
		printf("\n");
	else
	{
		if (ft_strcmp(main->token->next->str, "-n") == 0 \
			&& ft_strlen(main->token->next->str) == 2)
			skip_one_tkn(main, &n_flag);
		while (main->token && main->token->type != END)
		{
			if (main->token->type == ARG && main->token->next \
			&& main->token->next->type != END)
				printf("%s ", main->token->str);
			else if (main->token->type == ARG)
				printf("%s", main->token->str);
			main->token = main->token->next;
		}
		if (!n_flag)
			printf("\n");
	}
	return (1);
}
