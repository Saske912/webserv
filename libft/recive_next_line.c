/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpersimm <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/16 17:37:10 by wpersimm          #+#    #+#             */
/*   Updated: 2021/05/10 14:08:51 by wpersimm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int		recive_next_line(int fd, char **line)
{
	static char	*buf;
	char		bufer[ft_bufersize(BUFFER_SIZE) + 1];
	int			z;

	if (BUFFER_SIZE <= 0 || !line || fd < 0)
		return (0);
	z = 1;
	while (!ft_newline(buf, 0) && z != -1)
	{
		if ((z = recv(fd, bufer, BUFFER_SIZE, 0)) == 0)
			return (0);
		bufer[z] = 0;
		if (!(buf = ft_strjoiner(buf, bufer)))
			return (0);
	}
	if (!(*line = ft_strdupper(buf)))
		return (0);
	buf = ft_strduplic(buf);
	if (z != -1 && !buf)
		return (0);
	return (z == -1 ? z : 1);
}
