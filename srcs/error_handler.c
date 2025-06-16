/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 10:45:29 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/16 11:45:30 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	syntax_error(t_msh *msh)
{
	ft_putstr_fd("shell: syntax error near unexpected token\n", STDERR_FILENO);
	msh->ex_status = 258;
	return (1);
}

void	exit_shell(t_msh *msh)
{
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	rl_clear_history();
	ft_freeenvp(msh->envcp);
	exit(msh->ex_status);
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

int	handle_null_tokens(t_msh *msh, char *input)
{
	if (msh->tkn != NULL)
		return (1);
	if (input && is_blank_line(input))
		msh->ex_status = 0;
	else
		msh->ex_status = 2;
	free(input);
	free_tokens(msh->tkn);
	return (0);
}
