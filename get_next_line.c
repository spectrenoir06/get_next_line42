/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adoussau <antoine@doussaud.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/13 14:13:14 by adoussau          #+#    #+#             */
/*   Updated: 2014/11/14 12:13:22 by adoussau         ###   ########.fr       */
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

void	printline(t_list *lst, int pos)
{
	int		local = 0;
	int		posstart = pos;
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
	
	while (lst)
	{
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
		localcontent++;
	}
}


int		main()
{
	int		fd;
	int		ret;
	char	buff[BUFF_SIZE];
	t_list	*lst = NULL;
	char	*str;
	int	i=0;

	fd = open("42", 'r');
	if (fd == -1)
		return (1);
	while (ret = read(fd, buff, BUFF_SIZE))
		ft_lstpushback(&lst, ft_lstnew(buff, ret));

	printline(lst, 0);

	//while (lst)
	//{
		//ft_putnbr(lst->content_size);
		//printf("\np = %p -> %p",lst, lst->next);
	//	print(lst->content,lst->content_size);
	//	lst = lst->next;
	//}
}
