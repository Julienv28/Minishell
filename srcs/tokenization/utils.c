/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opique <opique@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 10:39:23 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/10 15:42:40 by opique           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*res;

	if (!s2)
		s2 = "";
	res = ft_strjoin(s1, s2);
	free(s1);
	return (res);
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

char	*append_char(char *res, char c)
{
	char	current[2];
	char	*tmp;

	current[0] = c;
	current[1] = '\0';
	tmp = ft_strjoin(res, current);
	if (!tmp)
		return (NULL);
	free(res);
	return (tmp);
}
