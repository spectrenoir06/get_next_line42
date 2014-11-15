/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adoussau <antoine@doussaud.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/13 14:13:14 by adoussau          #+#    #+#             */
/*   Updated: 2014/11/15 22:26:55 by adoussau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

#include "libft/libft.h"

#define BUFF_SIZE 1

void	ft_lstpushback(t_list **start, t_list *new)
{
	t_list		*lst;

	lst = *start;
	if (!(*start))
		*start = new;
	else
	{
		while (lst->next)
			lst = lst->next;
		lst->next = new;
	}
}

void	ft_lstsmartpushback(t_list **start, t_list *new)
{
	static t_list	*memstart = NULL;
	static t_list	*memend = NULL;

	if (memstart && *start == memstart)
	{
		memend->next = new;
		memend = new;
	}
	else
	{
		ft_lstpushback(start, new);
		memstart = *start;
		memend = new;
	}
}

		int		len(t_list *lst, unsigned int local)
{
	char	*localcontent;
	int		ret;

	ret = 0;
	localcontent = (char *)lst->content + local;
	while (*localcontent != '\n')
	{
		ret++;
		localcontent++;
		if (++local == lst->content_size)
		{
			lst = lst->next;
			localcontent = (char *)(lst->content);
			local = 0;
		}
	}
	return (ret);
}

int		readline(t_list **lst, unsigned int *pos, char *str)
{
	unsigned int	local;
	char			*localcontent;

	local = *pos;
	localcontent = (char *)(*lst)->content + local;
	while (*localcontent != '\n')
	{
		*str++ = *localcontent++;
		if (++local == (*lst)->content_size)
		{
			*lst = (*lst)->next;
			localcontent = (char *)(*lst)->content;
			local = 0;
		}
	}
	*str = 0;
	local++;
	if (local == (*lst)->content_size)
	{
		*lst = (*lst)->next;
		local = 0;
	}
	*pos = local;
	return (1);
}

int		get_next_line(const int fd, char **line)
{
	static t_list		*lst = NULL;
	static unsigned int	pos = 0;
	static int			fini = 0;
	int					ret;
	char				buff[BUFF_SIZE];

	if (!lst && !fini)
		while ((ret = read(fd, buff, BUFF_SIZE)))
		{
			ft_lstsmartpushback(&lst, ft_lstnew((void *)buff, ret));
			fini = 1;
		}
	else if (!lst && fini)
		return (0);
	*line = (char *)malloc(sizeof(char) * (len(lst, pos) + 1));
	if (!line)
		return (-1);
	return (readline(&lst, &pos, *line));
}

int		main(void)
{
	int		fd;
	char	*str;

	fd = open("file.txt", 'r');
	while (get_next_line(fd, &str))
	{
		//printf("<%s>\n", str);
		free(str);
		str = NULL;
	}
	return (0);
}
