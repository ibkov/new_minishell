/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burswyck <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/08 15:40:13 by burswyck          #+#    #+#             */
/*   Updated: 2020/11/08 15:51:14 by burswyck         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

typedef struct s_list
{
	int		len_main_array;
	int		index;
	int		lc;
	int		i;

}	t_list;

void	ft_free_mem(char **main_array, int main_index)
{
	while (main_index >= 0)
	{
		free(main_array[main_index]);
		main_index--;
	}
	free(main_array);
}

int	ft_count_words(char const *s, char c)
{
	int	index;
	int	count_words;

	index = 0;
	count_words = 0;
	if (s[0] != c)
		count_words++;
	while (s[index + 1])
	{
		if (s[index] == c && s[index + 1] != c)
			count_words++;
		index++;
	}
	return (count_words);
}

char	**mem_words(char **main_array, char const *s, char c, t_list main)
{
	main.lc = 0;
	main.i = 0;
	while (s[main.index] && main.index <= (int)ft_strlen(s))
	{
		while (s[main.index] != c && s[main.index])
		{
			main.lc++;
			main.index++;
		}
		if (main.lc != 0)
		{
			main_array[main.i] = (char *)malloc(sizeof(char) * (main.lc + 1));
			if (!(main_array[main.i]))
			{
				ft_free_mem(main_array, main.i);
				return (0);
			}
			main.i++;
		}
		main.lc = 0;
		main.index++;
	}
	return (main_array);
}

char	**collect_data(char **main_array, char const *s, char c)
{
	int		index;
	int		main_index;
	int		len_chars;

	len_chars = 0;
	index = 0;
	main_index = 0;
	while (s[index] && index <= (int)ft_strlen(s))
	{
		while (s[index] != c && s[index])
		{
			main_array[main_index][len_chars] = s[index];
			len_chars++;
			index++;
		}
		if (len_chars != 0)
		{
			main_array[main_index][len_chars] = '\0';
			main_index++;
		}
		len_chars = 0;
		index++;
	}
	main_array[main_index] = 0;
	return (main_array);
}

char	**ft_split(char const *s, char c)
{
	char	**main_array;
	t_list	main;

	main.index = 0;
	if (!s)
		return (0);
	main.len_main_array = ft_count_words(s, c);
	main_array = (char **)malloc(sizeof(char *) * (main.len_main_array + 1));
	if (!(main_array))
		return (0);
	main_array = mem_words(main_array, s, c, main);
	main_array = collect_data(main_array, s, c);
	return (main_array);
}
