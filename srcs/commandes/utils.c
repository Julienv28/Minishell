/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opique <opique@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 14:23:42 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/10 15:38:37 by opique           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
/*
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
		printf("expanded = %s\n", expanded);
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
}*/

char	*prepare_export_string(char *arg, char **envp, char **key, char **value)
{
	char	*equal;
	char	*replaced;

	*key = NULL;
	*value = NULL;
	replaced = replace_all_variables(arg, envp, 0, 1);
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
