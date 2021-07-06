/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmyesha <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/05 18:52:33 by dmyesha           #+#    #+#             */
/*   Updated: 2021/01/08 17:20:21 by dmyesha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int		free_str(char **str, int error)
{
	if (error == -1)
	{
		free(*str);
		*str = NULL;
		return (-1);
	}
	else
	{
		free(*str);
		*str = NULL;
	}
	return (0);
}

char	*check_remainder(char *reaminder, char **line)
{
	char *p_n;

	p_n = NULL;
	if (reaminder)
		if ((p_n = ft_strchr(reaminder, '\n')))
		{
			*p_n = '\0';
			*line = ft_strjoin(reaminder, "");
			ft_strcp(reaminder, ++p_n);
		}
		else
		{
			*line = ft_strjoin(reaminder, "");
			while (*reaminder)
				*reaminder++ = '\0';
		}
	else
		*line = ft_strnew(1);
	return (p_n);
}

void	str_join(char **str1, char **str2, int flag)
{
	char *tmp;

	if (flag == 1)
	{
		tmp = *str1;
		*str1 = ft_strjoin(*str1, *str2);
		free_str(&tmp, 0);
		free_str(str2, 0);
	}
	else if (flag == 2)
	{
		**str1 = '\0';
		++*str1;
		tmp = *str2;
		*str2 = ft_strjoin(*str1, "");
		free_str(&tmp, 0);
	}
}

int		get_next_line(int fd, char **line)
{
	char			*buf;
	int				byte_was_read;
	char			*p_n;
	static char		*remainder;

	byte_was_read = 1;
	if (fd < 0 || !line || BUFFER_SIZE <= 0)
		return (-1);
	p_n = check_remainder(remainder, line);
	while (!p_n && byte_was_read > 0)
	{
		if (!(buf = malloc(sizeof(char) * (BUFFER_SIZE + 1)))
				|| (byte_was_read = read(fd, buf, BUFFER_SIZE)) == -1)
			return (free_str(&buf, -1));
		buf[byte_was_read] = '\0';
		if ((p_n = ft_strchr(buf, '\n')))
			str_join(&p_n, &remainder, 2);
		str_join(line, &buf, 1);
	}
	return ((byte_was_read == 0) ? free_str(&remainder, 0) : 1);
}
