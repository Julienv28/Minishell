/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 14:23:42 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/16 14:33:41 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*export_s(char *arg, t_msh *msh, char **key, char **value)
{
	char	*equal;
	char	*replaced;

	*key = NULL;
	*value = NULL;
	replaced = replace_var(arg, msh, 0, 1);
	if (!replaced)
		return (NULL);
	equal = ft_strchr(replaced, '=');
	if (equal)
	{
		*key = ft_substr(replaced, 0, equal - replaced);
		*value = ft_strdup(equal + 1);
	}
	else
	{
		*key = ft_strdup(replaced);
		*value = NULL;
	}
	return (replaced);
}
