/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opique <opique@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 16:08:55 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/10 15:42:56 by opique           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*concat_command(char *current_command, char *new_part)
{
	char	*new_command;
	int		len_current;
	int		len_new;

	if (current_command)
		len_current = ft_strlen(current_command);
	else
		len_current = 0;
	len_new = ft_strlen(new_part);
	new_command = malloc(len_current + len_new + 2);
	if (new_command == NULL)
		return (NULL);
	if (current_command && *current_command)
	{
		ft_strcpy(new_command, current_command);
		ft_strcat(new_command, " ");
	}
	else
		new_command[0] = '\0';
	ft_strcat(new_command, new_part);
	free(current_command);
	return (new_command);
}

void	free_file_list(t_file_list *list)
{
	t_file_list	*tmp;

	while (list)
	{
		tmp = list;
		list = list->next;
		if (tmp->filename)
			free(tmp->filename);
		free(tmp);
	}
}

int	is_builting(char *cmd)
{
	return (
		ft_strcmp(cmd, "exit") == 0
		|| ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0);
}
