/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opique <opique@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 10:39:23 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/04 15:17:21 by opique           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Liberer les tokens
void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		if (tmp->value)
		{
			free(tmp->value);
			tmp->value = NULL;
		}
		free(tmp);
	}
}

static void	free_file_list_2(t_file_list *files)
{
	t_file_list	*tmp;

	while (files)
	{
		tmp = files->next;
		if (files->filename)
			free(files->filename);
		free(files);
		files = tmp;
	}
}

void	free_cmd(t_com_list *command)
{
	t_com_list	*tmp;

	while (command)
	{
		tmp = command->next;
		if (command->command)
			free(command->command);
		if (command->infile)
			free(command->infile);
		if (command->outfile)
			free(command->outfile);
		if (command->errfile)
			free(command->errfile);
		if (command->args)
			free_tab(command->args);
		if (command->all_outfilles)
			free_file_list_2(command->all_outfilles);
		if (command->heredoc_fd > 0)
			close(command->heredoc_fd);
		free(command);
		command = tmp;
	}
}
/*
void apply_redirection(t_com_list *curr_cmd, char *expanded, int redir_type)
{
    if (!curr_cmd || !expanded)
        return ;

    if (redir_type == INPUT)
    {
        // Libérer l'ancien infile s'il existe
        if (curr_cmd->infile)
            free(curr_cmd->infile);
        curr_cmd->infile = strdup(expanded);
    }
    else if (redir_type == APPEND)
    {
        // Libérer l'ancien outfile s'il existe
        if (curr_cmd->outfile)
            free(curr_cmd->outfile);
        curr_cmd->outfile = strdup(expanded);

        // Définir le mode append ou écriture simple
        curr_cmd->append_mode = (redir_type == APPEND) ? 1 : 0;
    }
}*/
