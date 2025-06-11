/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_manager.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oceanepique <oceanepique@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:33:17 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/11 16:56:47 by oceanepique      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	has_pipe(t_com *command)
{
	while (command)
	{
		if (command->is_pipe)
			return (1);
		command = command->next;
	}
	return (0);
}

int	parse_pipes(char **args)
{
	int	count;
	int	i;

	if (!args)
		return (0);
	i = 0;
	count = 0;
	while (args && args[i])
	{
		if (strcmp(args[i], "|") == 0)
			count++;
		i++;
	}
	return (count);
}
