/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oceanepique <oceanepique@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 15:19:26 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/11 10:02:57 by oceanepique      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

int	check_pipe(char *str, int i)
{
	int	j;

	if (!str)
		return (-1);
	if (str[i] == '|' && str[i + 1] == '|' && str[i + 1])
	{
		return (printf("Erreur : unexpected syntax near token `||'\n"), -1);
	}
	if (i == 0)
	{
		printf("Erreur : syntax error near unexpected token `%c'\n", str[i]);
		return (-1);
	}
	j = i + 1;
	while (str[j] && str[j] == ' ')
		j++;
	if (str[j] == '\0')
	{
		printf("Erreur : syntax error near unexpected token `newline'\n");
		return (-1);
	}
	if (str[j] == '|')
		return (printf("Erreur : syntax error near unexpected token `%c'\n", \
			str[j]), -1);
	return (0);
}

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
