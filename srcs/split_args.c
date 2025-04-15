/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opique <opique@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:06:45 by juvitry           #+#    #+#             */
/*   Updated: 2025/04/15 11:46:09 by opique           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_quote(char c)
{
	return (c == '"' || c == '\'');
}

static int	count_words(const char *s, char sep)
{
	int i = 0, count = 0, in_quote = 0;

	while (s[i])
	{
		while (s[i] == sep)
			i++;
		if (!s[i])
			break;
		count++;
		while (s[i] && (in_quote || s[i] != sep))
		{
			if (is_quote(s[i]))
				in_quote = !in_quote;
			i++;
		}
	}
	return (count);
}

static char	*word_dup(const char *s, int start, int end)
{
	char *res = malloc(end - start + 1);
	int i = 0;
	if (!res)
		return (NULL);
	while (start < end)
		res[i++] = s[start++];
	res[i] = '\0';
	return (res);
}

char	**split_args(const char *s, char sep)
{
	int		i = 0, j = 0, start;
	int		in_quote = 0;
	char	**tab;

	tab = malloc(sizeof(char *) * (count_words(s, sep) + 1));
	if (!tab)
		return (NULL);

	while (s[i])
	{
		while (s[i] == sep)
			i++;
		if (!s[i])
			break;
		start = i;
		while (s[i] && (in_quote || s[i] != sep))
		{
			if (is_quote(s[i]))
				in_quote = !in_quote;
			i++;
		}
		tab[j++] = word_dup(s, start, i);
	}
	tab[j] = NULL;
	return (tab);
}