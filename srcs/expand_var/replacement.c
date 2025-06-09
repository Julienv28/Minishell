/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replacement.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 15:35:55 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/09 15:49:39 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*handle_single_quotes(char *str, char *res, t_expand *var)
{
	(*var->i)++;
	(*var->i)++;

	if (str[*var->i] == '\'')
	{
		(*var->i)++;
		return (res);
	}
	while (str[*var->i] && str[*var->i] != '\'')
		res = append_char(res, str[(*var->i)++]);
	if (str[*var->i] == '\'')
		(*var->i)++;
	return (res);
}

static char	*handle_double_quotes(char *str, char *res, t_expand *var)
{
	if (str[*var->i + 2] == '"')
	{
		(*var->i) += 3;
		return (res);
	}
	else
	{
		(*var->i) += 2;
		while (str[*var->i] && str[*var->i] != '"')
			res = append_char(res, str[(*var->i)++]);
		if (str[*var->i] == '"')
			(*var->i)++;
		return (res);
	}
}

char	*replace_variable_or_special(char *str, char *res, t_expand *var)
{
	char	next;

	if (str[*var->i] == '$' && str[*var->i + 1] == '\'')
		return (handle_single_quotes(str, res, var));
	if (str[*var->i] == '$' && str[*var->i + 1] == '"')
		return (handle_double_quotes(str, res, var));
	(*var->i)++;
	if (!str[*var->i])
		return (append_char(res, '$'));
	next = str[*var->i];
	if (!(ft_isalpha(next) || ft_isdigit(next) || next == '_' || next == '{'
			|| next == '?'))
		return (append_char(res, '$'));
	if (next == '"' || next == '\'')
		return (res);
	if (next == '{')
		return (handle_brace_variable(str, res, var));
	if (ft_isalpha(next) || next == '_')
		return (expand_env_variable(str, res, var));
	if (ft_isdigit(next) || next == '?')
		return (handle_special_cases(str, res, var));
	return (append_char(res, '$'));
}
