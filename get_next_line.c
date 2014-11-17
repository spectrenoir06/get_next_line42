/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adoussau <antoine@doussaud.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/13 14:13:14 by adoussau          #+#    #+#             */
/*   Updated: 2014/11/17 17:09:39 by adoussau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include "libft/libft.h"
#include "get_next_line.h"

int		len(t_list *lst, unsigned int local)
{
	char	*localcontent;
	unsigned int		ret;

	ret = 0;
	localcontent = (char *)lst->content + local;
	while (*localcontent != '\n')
	{
		ret++;
		localcontent++;
		if (++local == lst->content_size)
		{
			lst = lst->next;
			if (!lst)
				break;
			localcontent = (char *)(lst->content);
			local = 0;
		}
	}
	return (ret);
}

int		readline(t_list **lst, unsigned int *pos, char *str)
{
	char			*localcontent;

	localcontent = (char *)(*lst)->content + *pos;
	while (*localcontent != '\n')
	{
		*str++ = *localcontent++;
		if (++(*pos) == (*lst)->content_size)
		{
			*lst = (*lst)->next;
			if (!(*lst))
				return (1);
			localcontent = (char *)((*lst)->content);
			*pos = 0;
		}
	}
	*str = 0;
	*pos++;
	if (++(*pos) == (*lst)->content_size)
	{
		*lst = (*lst)->next;
		*pos = 0;
	}
	return (1);
}

int		get_next_line(const int fd, char **line)
{
	static t_list		*lst = NULL;
	static unsigned int	pos = 0;
	static int			fini = 0;
	int					ret;
	char				buff[BUFF_SIZE];

	if (!line || !(*line) || BUFF_SIZE <= 0 || !fd)
		return (-1);
	if (!lst && !fini)
	{
		while (ret = read(fd, buff, BUFF_SIZE))
		{
			if (ret == 0)
				return (-1);
			ft_lstsmartpushback(&lst, ft_lstnew((void *)buff, ret));
		}
		fini = 1;
	}
	else if (!lst && fini)
	{
		ft_lstsimpledel(&lst);
		return (0);
	}
	*line = (char *)malloc(sizeof(char) * (len(lst, pos) + 1));
	return (readline(&lst, &pos, *line));
}
