/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_pipes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 11:35:17 by juvitry           #+#    #+#             */
/*   Updated: 2025/05/06 11:44:11 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

char	**split_pipe_respect_quotes(const char *line)
{
	char	**result;
	int		count = 0;
	int		i = 0, start = 0;
	char	quote = 0;

	while (line[i])
	{
		if (is_quote(line[i]))
		{
			quote = line[i++];
			while (line[i] && line[i] != quote)
				i++;
		}
		else if (line[i] == '|')
			count++;
		i++;
	}
	count++;
	result = malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);
	i = 0;
	int j = 0;
	start = 0;
	quote = 0;
	while (line[i])
	{
		if (is_quote(line[i]))
		{
			quote = line[i++];
			while (line[i] && line[i] != quote)
				i++;
		}
		else if (line[i] == '|')
		{
			result[j++] = ft_substr(line, start, i);
			start = i + 1;
		}
		i++;
	}
	result[j++] = ft_substr(line, start, i);
	result[j] = NULL;
	return (result);
}
