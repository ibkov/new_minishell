/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burswyck <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 17:53:28 by burswyck          #+#    #+#             */
/*   Updated: 2020/11/11 17:53:32 by burswyck         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int get_next_line(int fd, char **line)
{
	char buffer[100];
	**line = (char**)malloc(sizeof(char*)* 2);
	*line = (char*)malloc(sizeof(char*) * 15);
	while (read(fd, buffer, 10))
	{
		*line = buffer;
		line++;
	}
	return (0);
}