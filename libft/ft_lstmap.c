/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpersimm <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 22:50:47 by wpersimm          #+#    #+#             */
/*   Updated: 2020/11/01 23:18:25 by wpersimm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*kek;
	t_list	*lol;

	kek = NULL;
	if (lst == NULL)
		return (NULL);
	kek = ft_lstnew((*f)(lst->content));
	if (!kek)
		return (NULL);
	lol = kek;
	if (lst->next == NULL)
		return (lol);
	lst = lst->next;
	while (lst)
	{
		kek = ft_lstnew((*f)(lst->content));
		if (!kek)
			ft_lstclear(&lol, del);
		else
			ft_lstadd_back(&lol, kek);
		lst = lst->next;
	}
	return (lol);
}
