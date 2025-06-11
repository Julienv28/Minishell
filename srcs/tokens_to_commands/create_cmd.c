/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oceanepique <oceanepique@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 10:25:54 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/11 16:58:16 by oceanepique      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_com	*list_new(char *command)
{
	t_com	*new;

	new = malloc(sizeof(t_com));
	if (!new)
		return (NULL);
	if (command)
		new->cmd = ft_strdup(command);
	else
		new->cmd = NULL;
	new->args = NULL;
	new->is_pipe = 0;
	new->next = NULL;
	new->all_outfilles = NULL;
	new->heredoc_fd = -1;
	init_cmds(new);
	return (new);
}

t_com	*get_last(t_com *list)
{
	while (list && list->next != NULL)
		list = list->next;
	return (list);
}

void	add_bottom(t_com **list, t_com *new)
{
	t_com	*end_new;

	if (!new)
		return ;
	if (!*list)
	{
		*list = new;
		return ;
	}
	end_new = get_last(*list);
	end_new->next = new;
}

t_com	*fill_values(char **commands)
{
	t_com	*list;
	int		i;

	i = 0;
	if (commands[i] == NULL)
		return (0);
	while (commands[i] != NULL)
	{
		if (i == 0)
			list = list_new(commands[i]);
		else
			add_bottom(&list, list_new(commands[i]));
		i++;
	}
	return (list);
}

void	add_outfile(t_file_list **list, char *filename, int flag)
{
	t_file_list	*new;
	t_file_list	*tmp;

	new = malloc(sizeof(t_file_list));
	new->filename = ft_strdup(filename);
	new->flag = flag;
	new->next = NULL;
	if (*list == NULL)
		*list = new;
	else
	{
		tmp = *list;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}
