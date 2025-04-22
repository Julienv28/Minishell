/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opique <opique@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:06:45 by juvitry           #+#    #+#             */
/*   Updated: 2025/04/22 11:52:45 by opique           ###   ########.fr       */
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
	char	*raw_word;

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
		raw_word = word_dup(s, start, i);
		//tab[j++] = remove_quotes_or_slash(raw_word) NA PAS RETIRE LES QUOTE avant $
		tab[j++] = raw_word;
		//free(raw_word);
	}
	tab[j] = NULL;
	return (tab);
}

char	*remove_quotes_or_slash(char *str)
{
	int	i;
	int	j;
	char	*new_str;

	i = 0;
	j = 0;
	if (!str)
		return (NULL);
	new_str = malloc(sizeof(char) * ft_strlen(str) + 1);
	if (new_str == NULL)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\\' && str[i + 1])  // si on rencontre un \, on prend le caractère suivant
        {
            new_str[j++] = str[i + 1];
            i += 2;
        }
		if (str[i] != '\'' && str[i] != '"' && str[i] != '$') // Supprime quotes
		{
			new_str[j] = str[i];
			j++;
		}
		i++;
	}
	new_str[j] = '\0';
	return (new_str);
}