/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpersimm <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/31 11:56:21 by wpersimm          #+#    #+#             */
/*   Updated: 2020/11/02 12:11:31 by wpersimm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_minus(char *ar, int n, int i)
{
	int		z;

	n *= -1;
	i++;
	z = i;
	ar = malloc((i + 2) * sizeof(char));
	if (ar == NULL)
		return (NULL);
	while (i >= 1)
	{
		ar[i--] = n % 10 + 48;
		n /= 10;
	}
	ar[i] = 45;
	ar[z + 1] = 0;
	return (ar);
}

static char	*ft_plus(char *ar, int n, int i)
{
	int		z;

	z = i;
	ar = malloc((i + 2) * sizeof(char));
	if (ar == NULL)
		return (NULL);
	while (i >= 0)
	{
		ar[i--] = n % 10 + 48;
		n /= 10;
	}
	ar[z + 1] = 0;
	return (ar);
}

static char	*ft_minint(char *ar)
{
	ar = malloc(11 * sizeof(char) + 1);
	ar = ft_memcpy(ar, "-2147483648", 11);
	return (ar);
}

char		*ft_itoa(int n)
{
	char		*ar;
	int			i;
	long int	num;

	ar = NULL;
	if (n == -2147483648)
	{
		ar = ft_minint(ar);
		return (ar);
	}
	i = 0;
	num = n;
	while (n / 10)
	{
		n /= 10;
		i++;
	}
	if (n >= 0)
		ar = ft_plus(ar, num, i);
	else
		ar = ft_minus(ar, num, i);
	return (ar);
}
