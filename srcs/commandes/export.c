/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opique <opique@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 11:36:36 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/16 13:51:32 by opique           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_set_env(char *key, char *value, char ***envcp)
{
	int		i;
	char	*new_entry;
	char	**new_env;

	new_entry = build_env_entry(key, value);
	if (!new_entry)
		return ;
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
		return (free(new_entry));
	ft_freeenvp(*envcp);
	*envcp = new_env;
}

int	check_and_export(char *arg, char *key, char *value, char ***envcp)
{
	if (!is_valid_name(key))
	{
		if (ft_strchr(arg, '=') || !ft_isalpha(arg[0]))
		{
			ft_putstr_fd("bash: export: `", STDERR_FILENO);
			ft_putstr_fd(arg, STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
		}
		return (1);
	}
	if (ft_strchr(arg, '='))
		ft_set_env(key, value, envcp);
	return (0);
}

int	process_export(char *arg, char ***envcp, int *exit_status)
{
	char	*key;
	char	*value;
	char	*replaced;
	int		err;

	key = NULL;
	value = NULL;
	replaced = export_s(arg, *envcp, &key, &value);
	err = handle_export_error(replaced, arg);
	if (err)
	{
		if (err == 2)
			*exit_status = 2;
		else
			*exit_status = 1;
		return (1);
	}
	err = check_and_export(arg, key, value, envcp);
	if (err)
		*exit_status = 1;
	free_export_vars(key, value, replaced);
	return (0);
}

int	export_no_args(char **envp)
{
	int		i;
	char	*equal_sign;

	i = 0;
	while (envp[i])
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		equal_sign = ft_strchr(envp[i], '=');
		if (equal_sign)
		{
			write(STDOUT_FILENO, envp[i], equal_sign - envp[i]);
			ft_putstr_fd("=\"", STDOUT_FILENO);
			ft_putstr_fd(equal_sign + 1, STDOUT_FILENO);
			ft_putstr_fd("\"\n", STDOUT_FILENO);
		}
		else
		{
			ft_putstr_fd(envp[i], STDOUT_FILENO);
			ft_putchar_fd('\n', STDOUT_FILENO);
		}
		i++;
	}
	return (0);
}
