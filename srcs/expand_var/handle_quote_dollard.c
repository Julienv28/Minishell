/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quote_dollard.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oceanepique <oceanepique@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 15:59:22 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/11 17:07:40 by oceanepique      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*handle_opening_quote(char *str, int *quotes, int *i)
{
	if (str[*i] == '\'' && !quotes[1])
	{
		quotes[0] = !quotes[0];
		(*i)++;
		return ("");
	}
	if (str[*i] == '"' && !quotes[0])
	{
		quotes[1] = !quotes[1];
		(*i)++;
		return ("");
	}
	return (NULL);
}

static char	*handle_dollar_in_quotes(char *str, char *res, t_expand *var,
	int *quotes)
{
	if (str[*var->i] == '$' && quotes[1] && str[*var->i + 1] == '"')
	{
		(*var->i)++;
		return (append_char(res, '$'));
	}
	if (str[*var->i] == '$' && !quotes[0] && var->expand_vars)
	{
		var->quoted = quotes[1];
		return (replace_var_or_spe(str, res, var));
	}
	return (NULL);
}

char	*quote_dol(char *str, char *res,
	t_expand *var, int *quote)
{
	char	*tmp;

	tmp = handle_opening_quote(str, quote, var->i);
	if (tmp)
	{
		if (*tmp == '\0')
			return (res);
		return (tmp);
	}
	tmp = handle_dollar_in_quotes(str, res, var, quote);
	if (tmp)
		return (tmp);
	res = append_char(res, str[*var->i]);
	if (!res)
		return (NULL);
	(*var->i)++;
	return (res);
}
