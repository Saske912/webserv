/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpersimm <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/31 20:17:41 by wpersimm          #+#    #+#             */
/*   Updated: 2020/11/02 17:13:49 by wpersimm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*ar;
	size_t	i;
	size_t	j;

	if (!s1 || !s2)
		return (NULL);
	ar = malloc((ft_strlen(s1) + ft_strlen(s2)) * sizeof(char) + 1);
	if (ar == NULL)
		return (NULL);
	i = 0;
	j = 0;
	while (s1[i])
	{
		ar[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		ar[i] = s2[j];
		i++;
		j++;
	}
	ar[i] = 0;
	return (ar);
}
