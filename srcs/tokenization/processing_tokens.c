/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processing_tokens.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opique <opique@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:56:26 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/16 16:11:18 by opique           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	process_pipe(char *str, int *i, t_tkn **tkn, int *is_cmd, t_msh *msh)
{
	if (str[*i] == '|')
	{
		if (check_pipe(str, *i) == -1)
			return (msh->ex_status = 2, -1);
		add_token(tkn, "|", PIPE, 0);
		(*i)++;
		*is_cmd = 1;
		return (1);
	}
	return (0);
}

int	process_special_chars(char *str, int i)
{
	if (str[i] == '&' || str[i] == ':'
		|| str[i] == '!' || str[i] == '#' || str[i] == ';')
	{
		if (check_input(str, i) == -1)
			return (-1);
	}
	return (0);
}

int	process_redir(char *str, int *i, t_tkn **tkn, t_msh *msh)
{
	int	redirection_status;

	redirection_status = handle_redir(str, i, tkn, msh);
	if (redirection_status == -1)
		return (-1);
	return (redirection_status);
}

int	process_word(char **str, int *i, t_tkn **tkn, int *is_cmd)
{
	int	ret;

	ret = handle_word(str, i, tkn, is_cmd);
	if (ret == -1)
		return (-1);
	if (ret == 1)
		return (1);
	return (0);
}
