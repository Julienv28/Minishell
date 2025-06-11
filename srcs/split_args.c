/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oceanepique <oceanepique@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:06:45 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/11 14:54:50 by oceanepique      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_char(char *str, int i, char *dst, int single_quote)
{
	if (str[i] == '\\' && str[i + 1] && !single_quote)
	{
		dst[0] = str[i + 1];
		return (2);
	}
	else
	{
		dst[0] = str[i];
		return (1);
	}
}

int	update_quotes(char c, int *single_quote, int *double_quote)
{
	if (c == '\'' && !(*double_quote))
	{
		*single_quote = !(*single_quote);
		return (1);
	}
	if (c == '"' && !(*single_quote))
	{
		*double_quote = !(*double_quote);
		return (1);
	}
	return (0);
}

static void	copy_without_quotes(char *src, char *new_str)
{
	int	i;
	int	j;
	int	single_quote;
	int	double_quote;
	int	consumed;

	i = 0;
	j = 0;
	single_quote = 0;
	double_quote = 0;
	while (src[i])
	{
		if (update_quotes(src[i], &single_quote, &double_quote))
			i++;
		else
		{
			consumed = handle_char(src, i, &new_str[j], single_quote);
			i += consumed;
			j++;
		}
	}
	new_str[j] = '\0';
}

char	*remove_quotes_or_slash(char *str)
{
	char	*new_str;

	new_str = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!new_str)
		return (NULL);
	copy_without_quotes(str, new_str);
	return (new_str);
}
