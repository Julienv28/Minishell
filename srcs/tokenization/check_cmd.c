/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opique <opique@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 15:19:26 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/16 14:10:42 by opique           ###   ########.fr       */
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
		print_error_token_fd("Erreur : syntax error near unexpected token \
`newline'", STDERR_FILENO);
		return (-1);
	}
	if (str[*i] == '<' || str[*i] == '>' || str[*i] == '|')
	{
		if (((str[*i] == '>' && str[*i + 1] == '>')
				|| (str[*i] == '<' && str[*i + 1] == '<'))
			&& str[*i + 1])
			print_fd("Erreur : syntax error near unexpected token \
`", str[*i], 2, STDERR_FILENO);
		else if (str[*i] == '>' && *i > 0 && str[*i - 1] == '<')
			print_error_token_fd("Erreur : syntax error near unexpected token \
`newline'", STDERR_FILENO);
		else
			print_fd("Erreur : error token `", str[*i], 1, STDERR_FILENO);
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
		return (print_error_token_fd("Erreur : unexpected syntax near \
token `||'", STDERR_FILENO), -1);
	if (i == 0)
	{
		return (print_fd("Erreur : syntax error near unexpected token \
`", str[i], 1, STDERR_FILENO), -1);
	}
	j = i + 1;
	while (str[j] && str[j] == ' ')
		j++;
	if (str[j] == '\0')
		return (print_error_token_fd("Erreur : syntax error near unexpected \
token `newline'", STDERR_FILENO), -1);
	if (str[j] == '|')
	{
		print_fd("Erreur : syntax error near unexpected token \
`", str[j], 1, STDERR_FILENO);
		return (-1);
	}
	return (0);
}

int	check_input(char *str, int i)
{
	if (!str)
		return (-1);
	if ((str[i] == '&' && str[i + 1] == '&')
		|| (str[i] == ';' && str[i + 1] == ';'))
	{
		ft_putstr_fd("Erreur : syntax error near unexpected \
token `", STDERR_FILENO);
		ft_putchar_fd(str[i], STDERR_FILENO);
		ft_putchar_fd(str[i + 1], STDERR_FILENO);
		ft_putstr_fd("'\n", STDERR_FILENO);
		return (-1);
	}
	else if (str[i] == ':' || str[i] == '!' || str[i] == '#')
		return (-1);
	else
	{
		ft_putstr_fd("Erreur : syntax error near unexpected \
token `", STDERR_FILENO);
		ft_putchar_fd(str[i], STDERR_FILENO);
		ft_putstr_fd("'\n", STDERR_FILENO);
		return (-1);
	}
	return (0);
}
