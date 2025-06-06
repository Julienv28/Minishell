/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processing_tokens.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:56:26 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/06 15:59:03 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	process_pipe(char *str, int *i, t_token **tokens, int *expect_cmd)
{
	if (str[*i] == '|')
	{
		if (check_pipe(str, *i) == -1)
			return (-1);
		add_token(tokens, "|", PIPE);
		(*i)++;
		*expect_cmd = 1;
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

int	process_redirection(char *str, int *i, t_token **tokens, char **envcp)
{
	int	redirection_status;

	redirection_status = handle_redirection(str, i, tokens, envcp);
	if (redirection_status == -1)
		return (-1);
	return (redirection_status);
}

int	process_word(char **str, int *i, t_token **tokens, int *expect_cmd)
{
	if (handle_word(str, i, tokens, expect_cmd) == -1)
		return (-1);
	return (0);
}
