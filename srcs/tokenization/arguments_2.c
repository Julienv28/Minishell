/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arguments_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 16:13:44 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/06 16:14:00 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* VERSION QUI MACHE SANS HEREDOC
int	extract_word(char **str, int *i, char **word, int *start)
{
	while ((*str)[*i] && (*str)[*i] != ' ' && (*str)[*i] != '|' &&
			(*str)[*i] != '<' && (*str)[*i] != '>')
	{
		if ((*str)[*i] == '\'')
		{
			(*i)++;
			while ((*str)[*i] && (*str)[*i] != '\'')
				(*i)++;
			(*i)++;
		}
		else if ((*str)[*i] == '\"')
		{
			(*i)++;
			while ((*str)[*i] && (*str)[*i] != '\"')
				(*i)++;
			(*i)++;
		}
		else
			(*i)++;
	}
	*word = ft_strndup(*str + *start, *i - *start);
	if (!word)
		return (-1);
	return (0);
}*/

// VERSION QUI MACHE AVEC HEREDOC
int	extract_word(char **str, int *i, char **word, int *start)
{
	int		j;
	char	quote;
	int		has_quotes = 0;

	j = *i;

	if ((*str)[j] == '\'' || (*str)[j] == '"')
	{
		has_quotes = 1;
		quote = (*str)[j];
		j++;
		while ((*str)[j] && (*str)[j] != quote)
			j++;
		if ((*str)[j] == quote)
			j++;
	}
	else
	{
		while ((*str)[j] && (*str)[j] != ' ' && (*str)[j] != '|' &&
			(*str)[j] != '<' && (*str)[j] != '>')
			j++;
	}

	*word = ft_strndup(*str + *start, j - *start);
	if (!*word)
		return (-1);
	*i = j;
	return has_quotes; // ← on retourne ici 1 ou 0
}


int	update_str_with_input(char **str, char *input)
{
	char	*tmp;
	char	*join;

	tmp = ft_strjoin(*str, input);
	if (!tmp)
		return (free(input), -1);
	if (check_mismatched_quotes(tmp) == 1)
	{
		join = ft_strjoin(tmp, "\n");
		free(tmp);
	}
	else
		join = tmp;
	free(*str);
	*str = join;
	free(input);
	return (0);
}
