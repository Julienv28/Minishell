/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_manager.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:33:17 by juvitry           #+#    #+#             */
/*   Updated: 2025/05/06 11:46:35 by juvitry          ###   ########.fr       */
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

// int	parse_pipes(char **args)
// {
//     int count;
//     int i;

//     if (!args)
//         return (0);
//     i = 0;
//     count = 0;
//     while (args && args[i])
//     {
//         if (strcmp(args[i], "|") == 0)
//             count++;
//         i++;
//     }
//     return (count);
// }

// void	pipes_manager(t_com_list *command, int count, char **args, char **envcp)
// {
// 	int	fake_ac;

// 	fake_ac = count_ags(args);
// 	if (count == 1)
// 		pipex_simple(command, args, envcp);
// 	else
// 		complex_pipex(command, fake_ac, args, envcp);
// }
//fonction pour executer les pipes selon le nombre presents. 
// Besoin de revoir les args en parametre.
/*void	exec_pipes(t_minishell *mini)
{
	if (mini->have_pipes == 0)
		return ;
	else if (mini->have_pipes == 1)
		pipex_simple();
	else if (mini->have_pipes > 1)
		complex_pipex();
}*/

