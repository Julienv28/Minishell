/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 15:19:26 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/06 15:35:45 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Si on a une redirection qui est suivie d'un autre token spécial
// cas <>
int	check_redirection(char *str, int *i)
{
	if (!str)
		return (-1);
	while (str[*i] == ' ')
		(*i)++;
	if (!str[*i])
	{
		printf("Erreur : syntax error near unexpected token `newline'\n");
		return (-1);
	}
	if (str[*i] == '<' || str[*i] == '>' || str[*i] == '|')
	{
		if (((str[*i] == '>' && str[*i + 1] == '>')
				|| (str[*i] == '<' && str[*i + 1] == '<'))
			&& str[*i + 1])
			printf("Erreur : syntax error near unexpected token `%c%c'\n", \
					str[*i], str[*i]);
		else if (str[*i] == '>' && *i > 0 && str[*i - 1] == '<')
			printf("Erreur : syntax error near unexpected token `newline'\n");
		else
			printf("Erreur : syntax error near unexpected token `%c'\n", \
				str[*i]);
		return (-1);
	}
	return (0);
}

//Verifie la conformite de pipes (debut/fin de commande, ||...)
int	check_pipe(char *str, int i)
{
	int	j;

	if (!str)
		return (-1);
	if (str[i] == '|' && str[i + 1] == '|' && str[i + 1])
		return (printf("Erreur : syntax error near unexpected \
			token `||'\n"), -1);
	if (i == 0)
		return (printf("Erreur : syntax error near unexpected \
			token `%c'\n", str[i]), -1);
	j = i + 1;
	while (str[j] && str[j] == ' ')
		j++;
	if (str[j] == '\0')
		return (printf("Erreur : syntax error near unexpected token \
			`newline'\n"), -1);
	if (str[j] == '|')
		return (printf("Erreur : syntax error near unexpected token \
			`%c'\n", str[j]), -1);
	return (0);
}

// Vérifier si le caractère précédent est aussi un pipe
int	check_input(char *str, int i)
{
	if (!str)
		return (-1);
	if ((str[i] == '&' && str[i + 1] == '&')
		|| (str[i] == ';' && str[i + 1] == ';'))
	{
		printf("Erreur : syntax error near unexpected token `%c%c'\n", \
				str[i], str[i + 1]);
		return (-1);
	}
	else if (str[i] == ':' || str[i] == '!' || str[i] == '#')
		return (-1);
	else
	{
		printf("Erreur : syntax error near unexpected token `%c'\n", str[i]);
		return (-1);
	}
	return (0);
}
