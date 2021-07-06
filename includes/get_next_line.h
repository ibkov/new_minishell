/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmyesha <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/05 18:57:38 by dmyesha           #+#    #+#             */
/*   Updated: 2021/01/05 18:59:17 by dmyesha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
#define BUFFER_SIZE 10
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>

int			get_next_line(int fd, char **line);
size_t		ft_strlen(const char *s);
char		*ft_strcp(char *dest, const char *src);
char		*ft_strnew(size_t i);
char		*ft_strchr(const char *s, int c);
char		*ft_strjoin(char const *s1, char const *s2);

#endif
