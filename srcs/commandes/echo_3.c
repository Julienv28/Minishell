/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 11:20:29 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/10 11:23:54 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_backslashes(const char *str, int *i, char *res, int *j)
{
	int	count;
	int	pairs;

	count = 0;
	while (str[*i + count] == '\\')
		count++;
	pairs = count / 2;
	while (pairs--)
		res[(*j)++] = '\\';
	if (count % 2 == 1)
	{
		*i += count;
		if (str[*i])
			res[(*j)++] = str[(*i)++];
		return (1);
	}
	*i += count;
	return (0);
}

char	*unescape_backslashes(const char *str)
{
	int		i;
	int		j;
	char	*res;

	i = 0;
	j = 0;
	res = malloc(ft_strlen(str) + 1);
	if (!res)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\\')
			handle_backslashes(str, &i, res, &j);
		else
			res[j++] = str[i++];
	}
	res[j] = '\0';
	return (res);
}
