/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpersimm <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/29 09:30:26 by wpersimm          #+#    #+#             */
/*   Updated: 2020/11/02 20:50:39 by wpersimm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int		i;

	i = 0;
	if (c == 0)
	{
		while (s[i])
			i++;
	}
	else
	{
		while (s[i])
		{
			if (s[i] == c)
				break ;
			i++;
		}
		if (s[i] == 0)
			return (NULL);
	}
	return ((char *)s + i);
}
