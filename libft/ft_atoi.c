/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpersimm <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 17:31:29 by wpersimm          #+#    #+#             */
/*   Updated: 2020/10/31 09:44:06 by wpersimm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_isdig(int c)
{
	if (c > 47 && c < 58)
		return (1);
	return (0);
}

int			ft_atoi(const char *str)
{
	int						check;
	int						i;
	unsigned long int		res;

	res = 0;
	check = 1;
	i = -1;
	while (str[++i])
	{
		if (str[i] == 45 && ft_isdig(str[i + 1]) && !ft_isdig(str[i - 1]))
			check *= -1;
		else if (str[i] == 43 && ft_isdig(str[i + 1]) && !ft_isdig(str[i - 1]))
			continue;
		else if ((str[i] == ' ' || (str[i] > 8 && str[i] < 14)) && (i == 0 ||
		str[i - 1] == ' ' || (str[i - 1] > 8 && str[i - 1] < 14)))
			continue;
		else if (ft_isdig(str[i]))
			res = res * 10 + str[i] - 48;
		else
			break ;
	}
	(check == 1 && res > 9223372036854775807) ? res = (int)-1 : 0;
	(check == -1 && res > 9223372036854775807) ? res = (int)0 : 0;
	return ((int)res * check);
}
