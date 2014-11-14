/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adoussau <antoine@doussaud.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/13 14:13:14 by adoussau          #+#    #+#             */
/*   Updated: 2014/11/14 17:48:57 by adoussau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

#include "libft/libft.h"

#define BUFF_SIZE 5

int		get_next_line(const int fd, char **line)
{
	read(fd, *line, 25);
	*line[25] = 0;
}

void	print(void	*s, int		size)
{
	write(1,s,size);
	write(1,"\n",1);
}

void ft_lstpushback(t_list **start, t_list *new)
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

void	printline(t_list *lst, int posstart)
{
	int		local = 0;
	int		pos = posstart;
	char	*localcontent;
	while (pos--)
	{
		if (local++ > lst->content_size)
		{
			lst = lst->next;
			if (!lst)
				return;
			local = 0;
		}
	}
	localcontent = (char *)lst->content + local;
	
	while (*localcontent != '\n')
	{
		write(1,localcontent++,1);
		posstart++;
		local++;
		if (local == lst->content_size)
		{
			write(1, "|", 1);
			lst = lst->next;
			localcontent = (char *)lst->content;
			local = 0;
		}
	}
	printf("\npos = %d , newpos = %d\n", local, posstart+1);
}

int		len(t_list *lst, int local)
{
	char	*localcontent;
	int ret = 0;

	localcontent = (char *)lst->content + local;
	
	while (*localcontent != '\n')
	{
		ret++;
		localcontent++;
		if (++local == lst->content_size)
		{
			lst = lst->next;
			localcontent = (char *)lst->content;
			local = 0;
		}
	}
	return (ret);
}


void		readline(t_list **lst, int *pos, char	*str)
{
	int		local = *pos;
	char	*localcontent;
	char	*ret = str;


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
	if (++local == (*lst)->content_size)
	{
		*lst = (*lst)->next;
		local = 0;
	}
	*str = 0;
	*pos = local;
}


int		main()
{
	int		fd;
	int		ret;
	char	buff[BUFF_SIZE];
	t_list	*lst = NULL;
	char	*str;

	t_list	*locallst;
	int		localpos = 0;

	fd = open("42", 'r');
	if (fd == -1)
		return (1);
	while (ret = read(fd, buff, BUFF_SIZE))
		ft_lstpushback(&lst, ft_lstnew((void *)buff, ret));

	locallst = lst;
	while (locallst)
	{
		str = (char *)malloc(sizeof(char) * (len(locallst, localpos) + 1));
		readline(&locallst, &localpos, str);
		printf("%p, %d\n",locallst,localpos);
		printf("<%s>\n", str);
	}
}
