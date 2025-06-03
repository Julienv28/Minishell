/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 10:45:29 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/03 10:39:47 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	exit_error(void)
{
	perror("\033[31mError");
	exit (EXIT_FAILURE);
}

void	*free_and_return_null(char **pending_infile, char **pending_outfile)
{
	if (pending_infile)
	{
		free(pending_infile);
		pending_infile = NULL;
	}
	if (pending_outfile)
	{
		free(pending_outfile);
		pending_outfile = NULL;
	}
	return (NULL);
}

int	syntax_error(void)
{
	fprintf(stderr, "minishell: syntax error near unexpected token\n");
	g_exit_status = 258;
	return (1);
}
