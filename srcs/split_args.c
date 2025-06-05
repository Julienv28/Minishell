/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opique <opique@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:06:45 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/05 14:40:02 by opique           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"



char	*remove_quotes_or_slash(char *str)
{
	int	i;
	int	j;
	int	single_quote;
	int	double_quote;
	char	*new_str;

	i = 0;
	j = 0;
	single_quote = 0;
	double_quote = 0;
	new_str = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!new_str)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\'' && !double_quote)
		{
			single_quote = !single_quote;
			i++;
		}
		else if (str[i] == '"' && !single_quote)
		{
			double_quote = !double_quote;
			i++;
		}
		else if (str[i] == '\\' && str[i + 1] && !single_quote)
		{
			new_str[j++] = str[i + 1];
			i += 2;
		}
		else
			new_str[j++] = str[i++];
	}
	new_str[j] = '\0';
	return (new_str);
}
