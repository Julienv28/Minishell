/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:33:17 by juvitry           #+#    #+#             */
/*   Updated: 2025/04/04 14:43:20 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
void parse_pipes(t_com_list *list, t_minishell *mini)
{
    while (list->next != NULL)
    {
        if (ft_strncmp(list->command, "|", 1) == 0 && list->next != NULL)
        {
            list->is_pipe = 1;
            mini->have_pipes++;
        }
        else
        {
            list->is_pipe = 0;
        }
        list = list->next;
    }
    mini->have_pipes = 0;
}*/

void	parse_pipes(t_com_list *list, t_minishell *mini)
{
	while (list)
	{
		if (list->is_pipe)
			mini->have_pipes++;
		list = list->next;
	}
}

//fonction pour executer les pipes selon le nombre presents. 
// Besoin de revoir les args en parametre.
void	exec_pipes(t_minishell *mini)
{
	if (mini->have_pipes == 0)
		return ;
	else if (mini->have_pipes == 1)
		pipex_simple();
	else if (mini->have_pipes > 1)
		complex_pipex();
}
