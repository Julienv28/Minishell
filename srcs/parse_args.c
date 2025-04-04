/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oceanepique <oceanepique@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:33:17 by juvitry           #+#    #+#             */
/*   Updated: 2025/03/28 11:44:34 by oceanepique      ###   ########.fr       */
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

void parse_pipes(t_com_list *list, t_minishell *mini)
{
    while (list)
    {
        if (list->is_pipe)
            mini->have_pipes++;
        list = list->next;
    }
}
