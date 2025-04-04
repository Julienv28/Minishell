/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialisation_args.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oceanepique <oceanepique@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 10:25:54 by juvitry           #+#    #+#             */
/*   Updated: 2025/03/28 12:19:10 by oceanepique      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Crée une nouvelle commande dans la liste de commande
t_com_list *list_new(char *command)
{
    t_com_list *new;

    new = malloc(sizeof(t_com_list));
    if (!new)
        return (NULL);
    new->command = ft_strdup(command);
    new->is_pipe = 0;
    new->next = NULL;
    return (new);
}

// Trouve le dernier élément de la liste de commandes
t_com_list *get_last(t_com_list *list)
{
    while (list && list->next != NULL)
        list = list->next;
    return (list);
}

// Ajoute une nouvelle commande à la fin de la liste des commandes
void add_bottom(t_com_list **list, t_com_list *new)
{
    t_com_list *end_new;

    if (!new)
        return;
    if (!*list)
    {
        *list = new;
        return;
    }
    end_new = get_last(*list);
    end_new->next = new;
}

t_com_list *fill_values(char **commands)
{
    t_com_list *list;
    int i;

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
