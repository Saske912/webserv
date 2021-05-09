/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpersimm <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/29 16:37:20 by wpersimm          #+#    #+#             */
/*   Updated: 2020/11/01 20:07:53 by wpersimm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dst, const void *src, int c, size_t n)
{
	size_t	i;

	if (dst == 0 && src == 0)
		return (NULL);
	i = 0;
	while (i < n)
	{
		*((unsigned char *)(dst + i)) = *((unsigned char *)(src + i));
		i++;
		if (*((unsigned char *)src + i - 1) == (unsigned char)c)
			break ;
	}
	if (i == n && *((char *)src + i - 1) != (unsigned char)c)
		return (NULL);
	return (dst + i);
}
