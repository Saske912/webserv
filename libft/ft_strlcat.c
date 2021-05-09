/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpersimm <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/29 13:56:36 by wpersimm          #+#    #+#             */
/*   Updated: 2020/11/02 22:01:09 by wpersimm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	j;
	size_t	len;

	if (!dstsize)
		return (ft_strlen(src));
	len = ft_strlen(dst);
	if (len >= dstsize)
		return (dstsize + ft_strlen(src));
	i = 0;
	j = 0;
	while (i < dstsize - len - 1 && src[j])
	{
		dst[i + len] = src[j];
		i++;
		j++;
	}
	dst[i + len] = 0;
	return (len + ft_strlen(src));
}
