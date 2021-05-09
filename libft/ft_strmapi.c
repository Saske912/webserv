/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpersimm <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/31 20:57:58 by wpersimm          #+#    #+#             */
/*   Updated: 2020/11/02 17:13:03 by wpersimm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*ar;
	size_t	i;

	if (!s)
		return (NULL);
	ar = malloc(ft_strlen(s) * sizeof(char) + 1);
	if (ar == NULL)
		return (ar);
	i = 0;
	while (i < ft_strlen(s))
	{
		ar[i] = (*f)((unsigned int)i, s[i]);
		i++;
	}
	ar[i] = 0;
	return (ar);
}
