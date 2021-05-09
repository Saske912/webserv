/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpersimm <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/29 21:48:54 by wpersimm          #+#    #+#             */
/*   Updated: 2020/11/02 20:40:57 by wpersimm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t	i;

	if (n == 0)
		return (0);
	i = 0;
	while (*((unsigned char *)s1 + i) == *((unsigned char *)s2 + i)
	&& i < n - 1)
		i++;
	return (*((unsigned char *)s1 + i) - *((unsigned char *)s2 + i));
}
