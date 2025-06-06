/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 10:45:29 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/06 10:45:44 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	syntax_error(void)
{
	fprintf(stderr, "minishell: syntax error near unexpected token\n");
	g_exit_status = 258;
	return (1);
}

void	exit_shell(char **envcp)
{
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	rl_clear_history();
	ft_freeenvp(envcp);
	exit(g_exit_status);
}

int	is_blank_line(const char *str)
{
	while (*str)
	{
		if (*str != ' ' && *str != '\t')
			return (0);
		str++;
	}
	return (1);
}

int	handle_null_tokens(t_token *tokens, char *input)
{
	if (tokens != NULL)
		return (1);
	if (input && is_blank_line(input))
		g_exit_status = 0;
	else
		g_exit_status = 2;
	free(input);
	free_tokens(tokens);
	return (0);
}
