/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_building.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opique <opique@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 12:01:30 by opique            #+#    #+#             */
/*   Updated: 2025/06/10 15:39:53 by opique           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_export_vars(char *key, char *value, char *replaced)
{
	if (key && key != replaced)
		free(key);
	if (value && value != replaced && value != key)
		free(value);
	if (replaced)
		free(replaced);
}

void	ft_freeenvp(char **envcp)
{
	int	i;

	i = 0;
	if (!envcp)
		return ;
	while (envcp[i])
	{
		free(envcp[i]);
		i++;
	}
	free(envcp);
}

void	free_tab(char **tab)
{
	int	i;

	if (!tab)
		return ;
	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

char	*free_all(char *before, char *var_key, char *spaced, char *value)
{
	if (before)
		free(before);
	if (var_key)
		free(var_key);
	if (spaced)
		free(spaced);
	if (value)
		free(value);
	return (NULL);
}
