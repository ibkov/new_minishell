/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmyesha <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 19:05:35 by dmyesha           #+#    #+#             */
/*   Updated: 2021/07/08 19:05:37 by dmyesha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *command)
{
	if (ft_strcmp(command, "cd") == 0)
		return (1);
	if (ft_strcmp(command, "env") == 0)
		return (1);
	if (ft_strcmp(command, "export") == 0)
		return (1);
	if (ft_strcmp(command, "unset") == 0)
		return (1);
	if (ft_strcmp(command, "echo") == 0)
		return (1);
	if (ft_strcmp(command, "pwd") == 0)
		return (1);
	if (ft_strcmp(command, "$?") == 0)
		return (1);
	if (ft_strcmp(command, "exit") == 0)
		return (1);
	return (0);
}

void	execve_builtin(t_main *main)
{
	main->tokens = create_argv(main->token, 1, 0);
	redirect(main);
	if (ft_strncmp(main->token->str, "export", 6) == 0)
		sh_export(main);
	else if (ft_strncmp(main->token->str, "unset", 5) == 0)
		sh_unset(main);
	else if (ft_strncmp(main->token->str, "env", 3) == 0)
		sh_env(main, main->token->str);
	else if (ft_strncmp(main->token->str, "cd", 2) == 0)
		cd(main);
	else if (ft_strncmp(main->token->str, "pwd", 3) == 0)
		sh_pwd();
	else if (ft_strncmp(main->token->str, "echo", 4) == 0)
		sh_echo(main, 0);
	else if (ft_strncmp(main->token->str, "$?", 2) == 0)
	{
		printf("minishell: command not found: %d\n", g_sig.exit_status);
		g_sig.exit_status = 127;
		dup2(main->main_write, 1);
		return ;
	}
	else if (ft_strncmp(main->token->str, "exit", 4) == 0)
		sh_exit(main);
	dup2(main->main_write, 1);
}

int	is_bin(char *command, t_main *main)
{
	if (search_binary(command, main->envp, main))
		return (1);
	return (0);
}

void	execve_bin(t_main *main)
{
	main->tokens = create_argv(main->token, 1, 0);
	g_sig.pid = fork();
	if (g_sig.pid == 0)
	{
		redirect(main);
		execve(main->unix_path, main->tokens, main->envp);
		printf("minishell: %s: command not found\n", main->tokens[0]);
		exit(0);
	}
	else if (g_sig.pid > 0)
	{
		wait(&g_sig.exit_status);
		g_sig.exit_status = WEXITSTATUS(g_sig.exit_status);
	}
	else
		perror("Error fork\n");
}
