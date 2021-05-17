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

#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#define BUFFER_SIZE 1

static int		ft_newline(char *buf, int a)
{
    int		i;

    if (a == 1)
    {
        i = 0;
        if (!buf)
            return (0);
        while (buf[i])
            i++;
        return (i);
    }
    if (!buf)
        return (0);
    i = 0;
    while (buf[i])
    {
        if (buf[i + 1] && buf[i + 1] == 10 && buf[i] == '\r')
            return (i + 3);
        i++;
    }
    return (0);
}

static char	*ft_strdupper(char *buf)
{
    int		i;
    char	*ar;

    if (!buf)
        return (NULL);
    i = 0;
    while (buf[i] != '\r' && buf[i])
        i++;
    if (!(ar = malloc((i + 1) * sizeof(char))))
    {
        free(buf);
        return (NULL);
    }
    i = 0;
    while (buf[i] != '\r' && buf[i])
    {
        ar[i] = buf[i];
        i++;
    }
    ar[i] = 0;
    return (ar);
}

static char	*ft_strduplic(char *buf)
{
    int		i;
    int		j;
    char	*ar;

    if (!buf)
        return (NULL);
    i = 0;
    while (buf[i] != 10 && buf[i])
        i++;
    if (!buf[i])
    {
        free(buf);
        return (NULL);
    }
    if (!(ar = malloc((ft_newline(buf, 1) - i++) * sizeof(char))))
    {
        free(buf);
        return (NULL);
    }
    j = 0;
    while (buf[i])
        ar[j++] = buf[i++];
    ar[j] = 0;
    free(buf);
    return (ar);
}

static char	*ft_strjoiner(char *s1, char *s2)
{
    char	*ar;
    char	*lol;
    char	*kek;

    ar = malloc((ft_newline(s1, 1) + ft_newline(s2, 1) + 1) * sizeof(char));
    if (ar == NULL)
    {
        free(s1);
        return (NULL);
    }
    lol = ar;
    kek = s1;
    if (s1)
        while (*s1)
            *ar++ = *s1++;
    while (*s2)
        *ar++ = *s2++;
    *ar = 0;
    free(kek);
    return (lol);
}

static int		ft_bufersize(long int i)
{
    if (i <= 0)
        return (1);
    if (i >= 8384448)
        return (8384447);
    return (i);
}

int		recive_next_line(int fd, char **line)
{
	static char	*buf;
	char		bufer[ft_bufersize(BUFFER_SIZE) + 1];
	int			z;

	if (fd < 0)
		return (0);
	z = 1;
	while (!ft_newline(buf, 0) && z != -1)
	{
		if ((z = recv(fd, bufer, BUFFER_SIZE, 0)) == 0)
			return (0);
		if (z != -1)
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
