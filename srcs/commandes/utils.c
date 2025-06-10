/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opique <opique@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 14:23:42 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/10 12:20:54 by opique           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*prepare_export_string(char *arg, char **envp, char **key, char **value)
{
	char	*equal;
	char	*replaced;
	char	*expanded;
	char	*tmp;

	replaced = NULL;
	*key = NULL;
	*value = NULL;
	equal = ft_strchr(arg, '=');
	if (equal)
	{
		*key = ft_substr(arg, 0, equal - arg);
		*value = ft_strdup(equal + 1);
		expanded = replace_all_variables(*value, envp, 0, 1);
		free(*value);
		if (!expanded)
		expanded = ft_strdup("");
		*value = expanded;
		tmp = ft_strjoin(*key, "=");
		replaced = ft_strjoin(tmp, *value);
		free(tmp);
	}
	else
		replaced = ft_strdup(arg);
	return (replaced);
}
