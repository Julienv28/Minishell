/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quote_dollard.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 15:59:22 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/16 15:00:18 by juvitry          ###   ########.fr       */
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

static char	*handle_dollar_in_quotes(char *str, char *res, t_msh *msh,
	int *quotes)
{
	if (str[*msh->var->i] == '$' && quotes[1] && str[*msh->var->i + 1] == '"')
	{
		(*msh->var->i)++;
		return (append_char(res, '$'));
	}
	if (str[*msh->var->i] == '$' && !quotes[0] && msh->var->expand_vars)
	{
		msh->var->quoted = quotes[1];
		return (replace_var_or_spe(str, res, msh->var, msh));
	}
	return (NULL);
}

char	*quote_dol(char *str, char *res,
	t_msh *msh, int *quote)
{
	char	*tmp;

	tmp = handle_opening_quote(str, quote, msh->var->i);
	if (tmp)
	{
		if (*tmp == '\0')
			return (res);
		return (tmp);
	}
	tmp = handle_dollar_in_quotes(str, res, msh, quote);
	if (tmp)
		return (tmp);
	res = append_char(res, str[*msh->var->i]);
	if (!res)
		return (NULL);
	(*msh->var->i)++;
	return (res);
}
