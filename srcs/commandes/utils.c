/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oceanepique <oceanepique@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 14:23:42 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/11 12:44:28 by oceanepique      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
