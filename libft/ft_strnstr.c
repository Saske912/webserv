/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpersimm <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/29 09:33:11 by wpersimm          #+#    #+#             */
/*   Updated: 2020/10/31 09:39:14 by wpersimm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_strncmp2(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s2[i + 1] && s1[i] == s2[i])
		i++;
	return ((unsigned char)(s1[i]) - (unsigned char)(s2[i]));
}

char		*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;

	if (needle[0] == 0)
		return ((char *)haystack);
	i = 0;
	while (*(haystack + ft_strlen(needle) - 1) && i + ft_strlen(needle) <= len)
	{
		if (ft_strncmp2(haystack, needle) == 0)
			return ((char *)haystack);
		else
		{
			haystack++;
			i++;
		}
	}
	return (NULL);
}
