/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_manager.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:33:17 by juvitry           #+#    #+#             */
/*   Updated: 2025/05/06 13:08:46 by juvitry          ###   ########.fr       */
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

static void	remove_args(char **args, int index, int count)
{
	int	i;
	int	j;

	i = index;
	j = 0;
	while (j < count && args[i + j])
	{
		free(args[i + j]);
		args[i + j] = NULL;
		j++;
	}
	while (args[i + count])
	{
		args[i] = args[i + count];
		i++;
	}
	args[i] = NULL;
	while (args[i])
	{
		free(args[i]);
		args[i++] = NULL;
	}
}

static void	parse_redirections2(t_com_list *node)
{
	int		i;
	char	**args;

	i = 0;
	args = node->args;
	while (args && args[i])
	{
		if (ft_strcmp(args[i], "<") == 0 && args[i + 1])
		{
			node->infile = ft_strdup (args[i + 1]);
			remove_args(args, i, 2);
		}
		else if (ft_strcmp(args[i], ">") == 0 && args[i + 1])
		{
			node->outfile = ft_strdup(args[i + 1]);
			node->append = 0;
			remove_args(args, i, 2);
		}
		else if (ft_strcmp(args[i], ">>") == 0 && args[i + 1])
		{
			node->outfile = ft_strdup(args[i + 1]);
			node->append = 1;
			remove_args(args, i, 2);
		}
	}
}

t_com_list	*parse_pipes(char *line)
{
	t_com_list	*head = NULL, *current = NULL;
	char		**segments;
	int			i = 0;

	segments = split_pipe_respect_quotes(line);
	while (segments[i])
	{
		t_com_list	*node = malloc(sizeof(t_com_list));
		node->command = ft_strtrim(segments[i], " \t\n");
		node->args = split_args(node->command, ' ');
		node->infile = NULL;
		node->outfile = NULL;
		node->append = 0;
		node->next = NULL;
		parse_redirections2(node);
		if (!head)
			head = node;
		else
			current->next = node;
		current = node;
		i++;
	}
	free_tab(segments);
	return (head);
}
