/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmyesha <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 19:08:13 by dmyesha           #+#    #+#             */
/*   Updated: 2021/07/08 19:08:15 by dmyesha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	skip_one_tkn(t_main *main, int *n_flag)
{
	*n_flag = 1;
	main->token = main->token->next;
	main->token = main->token->next;
}

void	sh_echo(t_main *main, int	n_flag)
{
	t_token	*token;

	token = main->token;
	if (!token->next)
		printf("\n");
	else if (ft_strncmp(token->next->str, "$?", 2))
		printf("%d\n", g_sig.exit_status);
	else
	{
		if (ft_strcmp(token->next->str, "-n") == 0 \
			&& ft_strlen(token->next->str) == 2)
			skip_one_tkn(main, &n_flag);
		while (token && token->type != END)
		{
			if (token->type == ARG && token->next \
			&& token->next->type != END)
				printf("%s ", token->str);
			else if (token->type == ARG)
				printf("%s", token->str);
			token = token->next;
		}
		if (!n_flag)
			printf("\n");
	}
}
