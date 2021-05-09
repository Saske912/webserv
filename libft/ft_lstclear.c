/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpersimm <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 22:36:50 by wpersimm          #+#    #+#             */
/*   Updated: 2020/11/03 14:11:34 by wpersimm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*kek;
	t_list	*lol;

	if (!lst)
		return ;
	kek = *lst;
	while (kek)
	{
		lol = kek->next;
		(*del)(kek->content);
		free(kek);
		kek = lol;
	}
	*lst = NULL;
}
