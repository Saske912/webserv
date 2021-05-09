/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpersimm <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/29 22:49:48 by wpersimm          #+#    #+#             */
/*   Updated: 2020/10/31 09:37:57 by wpersimm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	size_t	i;
	char	*ar;

	i = 0;
	ar = (void *)malloc(count * size);
	if (NULL == ar)
		return (NULL);
	while (i < count * size)
		ar[i++] = 0;
	return (ar);
}
