/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmyesha <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 19:06:11 by dmyesha           #+#    #+#             */
/*   Updated: 2021/07/08 19:06:27 by dmyesha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*search_file(DIR *dir, char *file_path, char *command)
{
	struct dirent	*rd;
	char			*unix_path;
	char			*p;

	unix_path = NULL;
	rd = readdir(dir);
	while (rd != NULL)
	{
		if (ft_strncmp(rd->d_name, command, ft_strlen(rd->d_name)) == 0)
		{
			unix_path = ft_strjoin(file_path, "/");
			p = unix_path;
			unix_path = ft_strjoin(unix_path, command);
			free(p);
			break ;
		}
		rd = readdir(dir);
	}
	closedir(dir);
	return (unix_path);
}

int	search_in_path(char **envp, char *command, t_main *main, int i)
{
	char	*path;
	char	**bin_list;
	DIR		*dir;

	path = get_envi_val(envp, "PATH");
	if (!path)
		return (0);
	while (*path != '=')
		path++;
	bin_list = ft_split(path, ':');
	while (bin_list[i] != NULL)
	{
		dir = opendir(bin_list[i]);
		if (!(dir))
			break ;
		main->unix_path = search_file(dir, bin_list[i], command);
		if (main->unix_path != NULL)
			break ;
		i++;
	}
	if (main->unix_path == NULL)
		return (free_argv(bin_list, 0));
	return (free_argv(bin_list, 1));
}

int	search_in_command(char *command, t_main *main)
{
	DIR	*dir;
	int	i;

	if (*command != '/')
		return (0);
	i = ft_strlen(command);
	while (command[i] != '/')
		i--;
	command[i++] = '\0';
	dir = opendir(command);
	if (!(dir))
		return (0);
	main->unix_path = search_file(dir, command, command + i);
	if (main->unix_path == NULL)
		return (0);
	else
	{
		free(main->token->str);
		main->token->str = ft_strdup(command + i);
	}
	free(command);
	return (1);
}

int	search_in_current_dir(char **envp, char *command, t_main *main)
{
	DIR		*dir;
	char	*pwd;

	pwd = get_envi_val(envp, "PWD");
	pwd = pwd + 4;
	dir = opendir(pwd);
	if (!(dir))
		return (0);
	main->unix_path = search_file(dir, pwd, command);
	if (main->unix_path == NULL)
		return (0);
	return (1);
}

int	search_binary(char *command, char **envp, t_main *main)
{
	char	*tmp;

	tmp = ft_strdup(command);
	if ((ft_strncmp(command, "./", 2) == 0))
	{
		if (search_in_current_dir(envp, command + 2, main))
		{
			free(tmp);
			return (1);
		}
	}
	else if (search_in_path(envp, command, main, 1))
	{
		free(tmp);
		return (1);
	}
	else if (search_in_command(tmp, main))
	{
		free(tmp);
		return (1);
	}
	free(tmp);
	return (0);
}
