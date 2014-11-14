/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adoussau <antoine@doussaud.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/13 14:13:14 by adoussau          #+#    #+#             */
/*   Updated: 2014/11/14 20:22:42 by adoussau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

#include "libft/libft.h"

#define BUFF_SIZE 5

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
			localcontent = (char *)(lst->content);
			local = 0;
		}
	}
	return (ret);
}


int		readline(t_list **lst, int *pos, char	*str)
{
	int		local = *pos;
	char	*localcontent;
	char	*ret = str;


	localcontent = (char *)(*lst)->content + local;
	
	while (*localcontent != '\n' && *localcontent != EOF)
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
	if (*localcontent == EOF)
		return (0);
	if (++local == (*lst)->content_size)
	{
		*lst = (*lst)->next;
		local = 0;
	}
	*pos = local;
	return (1);
}

int		get_next_line(const int fd, char **line)
{
	static t_list	*lst = NULL;
	static int		pos = 0;
	int				ret;
	char	buff[BUFF_SIZE];

	if (!lst)
		while (ret = read(fd, buff, BUFF_SIZE))
			ft_lstpushback(&lst, ft_lstnew((void *)buff, ret));
	*line = (char *)malloc(sizeof(char) * (len(lst, pos) + 1));
	if (!line)
		return (-1);
	return (readline(&lst, &pos, *line));
}

int		main()
{
	int		fd = open("42", 'r');
	char	*str;

	while (get_next_line(fd, &str))
	{
		printf("<%s>\n", str);
	}
}

void	print(void	*s, int		size)
{
	write(1,s,size);
	write(1,"\n",1);
}
