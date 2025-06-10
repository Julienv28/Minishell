/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opique <opique@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 16:37:05 by opique            #+#    #+#             */
/*   Updated: 2025/06/10 16:37:06 by opique           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**copy_old_env(char **envcp, char **new_env, int size)
{
	int	j;

	j = 0;
	while (j < size)
	{
		new_env[j] = ft_strdup(envcp[j]);
		if (!new_env[j])
		{
			while (--j >= 0)
				free(new_env[j]);
			free(new_env);
			return (NULL);
		}
		j++;
	}
	return (new_env);
}

char	**ft_realloc_env(char **envcp, char *new_entry)
{
	int		i;
	char	**new_env;

	i = 0;
	while (envcp && envcp[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return (NULL);
	if (!copy_old_env(envcp, new_env, i))
		return (NULL);
	new_env[i] = new_entry;
	if (!new_env[i])
	{
		while (--i >= 0)
			free(new_env[i]);
		free(new_env);
		return (NULL);
	}
	new_env[i + 1] = NULL;
	return (new_env);
}

char	*build_env_entry(char *key, char *value)
{
	char	*tmp;
	char	*new_entry;

	tmp = ft_strjoin(key, "=");
	if (!tmp)
		return (NULL);
	if (value)
		new_entry = ft_strjoin(tmp, value);
	else
		new_entry = ft_strjoin(tmp, "");
	free(tmp);
	return (new_entry);
}

int	handle_export_error(char *replaced, char *arg)
{
	if (!replaced || replaced[0] == '\0')
	{
		ft_putstr_fd("bash: export: `", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
		return (1);
	}
	if (replaced[0] == '-')
	{
		ft_putstr_fd("bash: export: `", STDERR_FILENO);
		ft_putchar_fd('-', STDERR_FILENO);
		ft_putchar_fd(replaced[1], STDERR_FILENO);
		ft_putstr_fd("': invalid option\n", STDERR_FILENO);
		return (2);
	}
	return (0);
}

int	is_valid_name(char *name)
{
	int	i;

	if (!name || (!ft_isalpha(name[0]) && name[0] != '_'))
		return (0);
	i = 1;
	while (name[i])
	{
		if (!(ft_isalnum(name[i]) || name[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}
