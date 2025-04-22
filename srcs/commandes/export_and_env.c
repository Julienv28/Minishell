/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_and_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 11:36:36 by juvitry           #+#    #+#             */
/*   Updated: 2025/04/22 15:05:14 by juvitry          ###   ########.fr       */
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
            return (0);
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
	char	*equal_sign;

	if (!arg)
	{
		ft_env(*envcp);
		return ;
	}
	arg = replace_all_variables(arg);
	if (!arg)
		return ;
	if (arg[0] == '\0')
	{
		ft_putstr_fd("export: `': not a valid identifier\n", STDERR_FILENO);
		free(arg);
		return ;
	}
	if (arg[0] == '-')
	{
		printf("bash: export: -%c: invalid option\n", arg[1]);
		free(arg);
		return ;
	}
	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
	{
		key = ft_substr((const char *)arg, 0, equal_sign - arg);
		value = ft_strdup(equal_sign + 1);
	}
	else
	{
		key = ft_strdup(arg);
		value = NULL;
	}
	if (!is_valid_name(key))
	{
		ft_putstr_fd("export: `", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
		free(key);
		return ;
	}
	if (equal_sign)
		ft_set_env(key,value, envcp);
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
