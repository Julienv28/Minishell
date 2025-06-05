/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_pipes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opique <opique@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 11:35:17 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/05 14:53:55 by opique           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

int	count_pipes(const char *line)
{
	int	i;
	int	count;
	char	quote;

	i = 0;
	count = 0;
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
		i++;
	}
	return (count + 1);
}

void	fill_result(const char *line, char **result)
{
	int	i;
	int	start;
	int	j;
	char	quote;

	i = 0;
	start = 0;
	j = 0;
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
			result[j++] = ft_substr(line, start, i - start);
			start = i + 1;
		}
		i++;
	}
	result[j++] = ft_substr(line, start, i - start);
	result[j] = NULL;
}

char	**split_pipe_respect_quotes(const char *line)
{
	char	**result;
	int		count;

	if (!line)
		return (NULL);
	count = count_pipes(line);
	result = malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);
	fill_result(line, result);
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
