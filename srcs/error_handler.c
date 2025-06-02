/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 10:45:29 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/02 13:54:10 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	exit_error(void)
{
	perror("\033[31mError");
	exit (EXIT_FAILURE);
}

void *free_and_return_null(char **pending_infile, char **pending_outfile)
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
    return NULL;
}

int syntax_error(void)
{
    // Afficher un message d'erreur sur stderr
    fprintf(stderr, "minishell: syntax error near unexpected token\n");
    // Optionnel : set une variable d'état d'erreur globale
    // g_exit_code = 258; // Par exemple, comme bash

    // Retourner une valeur qui indique l'erreur, souvent 1
    return 1;
}
