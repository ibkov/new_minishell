/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmyesha <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 19:08:06 by dmyesha           #+#    #+#             */
/*   Updated: 2021/07/08 19:08:08 by dmyesha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	char	*cd_down(char *dir, char **envp)
{
	int	i;

	change_envp(envp, "OLDPWD=", dir);
	i = ft_strlen(dir);
	while (dir[i] != '/')
		i--;
	dir[i] = '\0';
	return (ft_strdup(dir));
}

static	char	*cd_rel(char *dir, char **envp, char *val)
{
	char	*tmp;

	change_envp(envp, "OLDPWD=", dir);
	tmp = val;
	val = ft_strjoin("/", val);
	tmp = dir;
	dir = ft_strjoin(dir, val);
	free(val);
	return (dir);
}

void	cd(t_main *main)
{
	char	*current_dir;
	char	*new_dir;

	current_dir = getcwd(NULL, 100);
	if (ft_strncmp(main->tokens[1], "..", 2) == 0)
		new_dir = cd_down(current_dir, main->envp);
	else if (ft_strncmp(main->tokens[1], "/", 1) == 0)
	{
		change_envp(main->envp, "OLDPWD=", current_dir);
		new_dir = ft_strdup(main->tokens[1]);
	}
	else if (ft_strncmp(main->tokens[1], ".", 1) == 0)
	{
		new_dir = ft_strdup(current_dir);
		change_envp(main->envp, "OLDPWD=", current_dir);
	}
	else
		new_dir = cd_rel(current_dir, main->envp, main->tokens[1]);
	chdir(new_dir);
	change_envp(main->envp, "PWD=", new_dir);
	free(new_dir);
	free(current_dir);
}
