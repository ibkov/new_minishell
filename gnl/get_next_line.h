/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burswyck <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 17:53:56 by burswyck          #+#    #+#             */
/*   Updated: 2020/11/11 17:54:02 by burswyck         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GNL_GET_NEXT_LINE_H
#define GNL_GET_NEXT_LINE_H

#include <stdlib.h>
#include <fcntl.h>
#include "libft/libft.h"

int get_next_line(int fd, char **line);

#endif //GNL_GET_NEXT_LINE_H
