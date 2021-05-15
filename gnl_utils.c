/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpersimm <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/16 22:49:26 by wpersimm          #+#    #+#             */
/*   Updated: 2021/01/10 15:58:50 by wpersimm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int		ft_newline(char *buf, int a)
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
		if (buf[i++] == 10)
			return (i + 1);
	return (0);
}

char	*ft_strdupper(char *buf)
{
	int		i;
	char	*ar;

	if (!buf)
		return (NULL);
	i = 0;
	while (buf[i] != 10 && buf[i])
		i++;
	if (!(ar = malloc((i + 1) * sizeof(char))))
	{
		free(buf);
		return (NULL);
	}
	i = 0;
	while (buf[i] != 10 && buf[i])
	{
		ar[i] = buf[i];
		i++;
	}
	ar[i] = 0;
	return (ar);
}

char	*ft_strduplic(char *buf)
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

char	*ft_strjoiner(char *s1, char *s2)
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

int		ft_bufersize(long int i)
{
	if (i <= 0)
		return (1);
	if (i >= 8384448)
		return (8384447);
	return (i);
}
