/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_pipes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opique <opique@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 11:35:17 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/04 16:54:50 by opique           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

static int	fill_pipe_splits(char **result, const char *line)
{
	int	i;
	int	j;
	int	start;
	char	quote;

	i = 0;
	j = 0;
	start = 0;
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
			result[j++] = ft_substr(line, start, i - start);
			if (!result[j - 1])
				return (-1);
			start = i + 1;
		}
		if (line[i])
			i++;
	}
	result[j++] = ft_substr(line, start, i - start);
	if (!result[j - 1])
		return (-1);
	result[j] = NULL;
	return (0);
}

static int	count_pipes(const char *line)
{
	int	i;
	int	count;
	char quote;

	i = 0;
	count = 1;
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
			count++;
		if (line[i])
			i++;
	}
	return (count);
}

char	**split_pipe_respect_quotes(const char *line)
{
	char	**result;
	int		count;

	count = count_pipes(line);
	result = malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);
	if (fill_pipe_splits(result, line) == -1)
	{
		free_tab(result);
		return (NULL);
	}
	return (result);
}

/*
char	**split_pipe_respect_quotes(const char *line)
{
	char	**result;
	int		count;
	int		i;
	int		j;
	int		start;
	char	quote;

	i = 0;
	start = 0;
	quote = 0;
	count = 0;
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
	j = 0;
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
}*/
