/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arguments_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 16:13:44 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/09 15:25:23 by juvitry          ###   ########.fr       */
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
	int		has_quotes;
	int		tab[2];

	has_quotes = 0;
	tab[0] = 0;
	tab[1] = 0;
	j = *i;
	while ((*str)[j])
	{
		if ((*str)[j] == '\'' && !tab[1])
			tab[0] = !tab[0];
		else if ((*str)[j] == '"' && !tab[0])
			tab[1] = !tab[1];
		else if (!tab[0] && !tab[1] &&
			(ft_isspace((*str)[j]) || (*str)[j] == '|' ||
			(*str)[j] == '<' || (*str)[j] == '>'))
			break ;
		j++;
	}
	*word = ft_strndup(*str + *start, j - *start);
	if (!*word)
		return (-1);
	*i = j;
	return (has_quotes || tab[0] || tab[1]);
}

int	update_str_with_input(char **str, char *input)
{
	char	*tmp;
	char	*join;

	tmp = ft_strjoin(*str, input);
	if (!tmp)
		return (-1);
	if (check_mismatched_quotes(tmp) == 1)
	{
		join = ft_strjoin(tmp, "\n");
		free(tmp);
		if (!join)
			return (-1);
	}
	else
		join = tmp;
	free(*str);
	*str = join;
	return (0);
}
