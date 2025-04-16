/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_and_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opique <opique@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 11:36:36 by juvitry           #+#    #+#             */
/*   Updated: 2025/04/16 15:47:12 by opique           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Fonction pour vérifier la validité du nom de la variable
int is_valid_name(char *name)
{
    int i;

    if (!name || (!ft_isalpha(name[0]) && name[0] != '_')) // Le nom doit commencer par une lettre ou un underscore
        return (0);
    i = 1;
    while (name[i])
    {
        if (!(ft_isalnum(name[i]) || name[i] == '_')) // Le reste doit être alphanumérique ou underscore
        {
            printf("restant lettres fausses\n");
            return (0);
        }
        i++;
    }
    return (1);
}

void	ft_set_env(char *key, char *value, char ***envcp)
{
	int		i;
	char	*new_entry;
	char	*tmp;
	char	**new_env;

	new_entry = ft_strjoin(key, "=");
	tmp = ft_strjoin(new_entry, value);
	free(new_entry);
	new_entry = tmp;
	i = 0;
	while (*envcp && (*envcp)[i])
	{
		if (ft_strncmp((*envcp)[i], key, ft_strlen(key)) == 0
			&& (*envcp)[i][ft_strlen(key)] == '=')
		{
			free((*envcp)[i]);
			(*envcp)[i] = new_entry;
			return ;
		}
		i++;
	}
	new_env = ft_realloc_env(*envcp, new_entry);
	if (!new_env)
	{
		free(new_entry);
		return ;
	}
	ft_freeenvp(*envcp);
	*envcp = new_env;
}


void	ft_export(char *arg, char ***envcp)
{
	char	*key;
	char	*value;

	if (!arg)
	{
		ft_env(*envcp);
		return ;
	}
	else if (arg && !ft_strchr(arg, '='))
		return ;
	key = ft_substr((const char *)arg, 0, ft_strchr(arg, '=') - arg);
	value = ft_strdup(ft_strchr(arg, '=') + 1);
	if (is_valid_name(key))
		ft_set_env(key, value, envcp);
	free(key);
	free(value);
}

void	ft_env(char **envcp)
{
	int	i;

	i = 0;
	while (envcp[i])
	{
		printf("%s\n", envcp[i]);
		i++;
	}
}
