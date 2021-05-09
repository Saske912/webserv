/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpersimm <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/03 15:37:23 by wpersimm          #+#    #+#             */
/*   Updated: 2021/01/27 19:55:17 by wpersimm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**ft_split(char	const *s, char c)
{
	char	**ar;
	size_t	i;
	size_t	j;
	size_t	y;

	if (!s)
		return (NULL);
	i = -1;
	y = 0;
	while (s[++i])
		if ((s[i] == c && (s[i + 1] != c && s[i + 1] != 0))
		|| (s[i] != c && s[i + 1] == 0))
			y++;
	if (!(ar = malloc((y + 1) * sizeof(char *))))
		return (NULL);
	y = -1;
	i = 0;
	j = 0;
	while (s[++y])
		if ((s[y] == c && (s[y + 1] != c && s[y + 1] != 0)))
			j = y + 1;
		else if (s[y] != c && (s[y + 1] == 0 || s[y + 1] == c))
			ar[i++] = ft_substr(s, j, y - j + 1);
	ar[i] = NULL;
	return (ar);
}
