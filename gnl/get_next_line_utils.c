/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmyesha <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/05 19:02:27 by dmyesha           #+#    #+#             */
/*   Updated: 2021/01/05 19:11:31 by dmyesha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char		*ft_strchr(const char *s, int c)
{
	unsigned char sym;

	sym = c;
	if (sym == '\0')
	{
		s = s + ft_strlen(s);
		return ((char *)s);
	}
	while (*s)
	{
		if (*s == sym)
			return ((char *)s);
		s++;
	}
	return (NULL);
}

char		*ft_strnew(size_t i)
{
	char	*dst;
	size_t	j;

	if (!(dst = (char *)malloc((i) * sizeof(char))))
		return (0);
	j = 0;
	while (j != i)
	{
		dst[j] = '\0';
		j++;
	}
	return (dst);
}

char		*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!s1 || !s2)
		return (0);
	str = (char *)malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!str)
		return (0);
	while (s1[i])
		str[j++] = s1[i++];
	i = 0;
	while (s2[i])
		str[j++] = s2[i++];
	str[j] = '\0';
	return (str);
}

size_t		ft_strlen(const char *s)
{
	int i;

	i = 0;
	while (*s++)
		i++;
	return (i);
}

char		*ft_strcp(char *dest, const char *src)
{
	unsigned int i;

	i = 0;
	while (src[i] != '\0')
	{
		*(dest + i) = *(src + i);
		i++;
	}
	*(dest + i) = '\0';
	return (dest);
}
