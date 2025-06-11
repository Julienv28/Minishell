/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processing_tokens.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oceanepique <oceanepique@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:56:26 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/11 17:12:26 by oceanepique      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	process_pipe(char *str, int *i, t_tkn **tkn, int *is_cmd)
{
	if (str[*i] == '|')
	{
		if (check_pipe(str, *i) == -1)
			return (-1);
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

int	process_redir(char *str, int *i, t_tkn **tkn, char **envcp)
{
	int	redirection_status;

	redirection_status = handle_redir(str, i, tkn, envcp);
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
