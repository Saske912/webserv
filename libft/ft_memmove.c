/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpersimm <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/29 19:06:36 by wpersimm          #+#    #+#             */
/*   Updated: 2020/10/30 19:15:57 by wpersimm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t	i;

	if (src == 0 && dst == 0)
		return (NULL);
	i = 0;
	if (src > dst)
		while (i < len)
		{
			*((char *)(dst + i)) = *((char *)(src + i));
			i++;
		}
	else
		while (len > 0)
		{
			*((char *)(dst + len - 1)) = *((char *)(src + len - 1));
			len--;
		}
	return (dst);
}
