/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpersimm <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 14:28:47 by wpersimm          #+#    #+#             */
/*   Updated: 2020/11/02 22:41:42 by wpersimm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_checker(char const *set, const char s)
{
	size_t	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == s)
			return (1);
		i++;
	}
	return (0);
}

char		*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;

	if (!s1 || !set)
		return (NULL);
	end = ft_strlen(s1) - 1;
	while (ft_checker(set, s1[end]))
		end--;
	end++;
	start = 0;
	while (ft_checker(set, s1[start]))
		start++;
	if (start == ft_strlen(s1))
		return (ft_substr("", 0, 1));
	return (ft_substr(s1, start, end - start));
}
