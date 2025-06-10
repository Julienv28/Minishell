/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opique <opique@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 11:36:36 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/10 16:36:49 by opique           ###   ########.fr       */
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
			printf("bash: export: `%s': not a valid identifier\n", arg);
		return (1);
	}
	if (ft_strchr(arg, '='))
		ft_set_env(key, value, envcp);
	return (0);
}

int	process_export_entry(char *arg, char ***envcp, int *exit_status)
{
	char	*key;
	char	*value;
	char	*replaced;
	int		err;

	key = NULL;
	value = NULL;
	replaced = prepare_export_string(arg, *envcp, &key, &value);
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
	int	i;
	int	j;

	i = 0;
	while (envp[i])
	{
		printf("declare -x ");
		j = 0;
		while (envp[i][j] && envp[i][j] != '=')
			putchar(envp[i][j++]);
		if (envp[i][j] == '=')
			printf("=\"%s\"\n", envp[i] + j + 1);
		else
			printf("\n");
		i++;
	}
	return (0);
}

/*
// Fonction principale
int	ft_export(char **args, char ***envcp)
{
	int	i;
	int	exit_status;

	i = 1;
	exit_status = 0;
	if (!args[1])
		return (export_no_args(*envcp));
	while (args[i])
	{
		process_export_entry(args[i], envcp, &exit_status);
		i++;
	}
	return (exit_status);
}*/
