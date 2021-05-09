/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpersimm <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 20:23:57 by wpersimm          #+#    #+#             */
/*   Updated: 2020/11/01 20:34:11 by wpersimm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*kek;

	kek = (t_list *)malloc(sizeof(t_list));
	if (kek == NULL)
		return (kek);
	kek->content = content;
	kek->next = NULL;
	return (kek);
}
