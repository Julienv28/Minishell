/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opique <opique@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 11:36:21 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/16 11:34:16 by opique           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	should_unset_var(char *env_entry, char **args)
{
	int		i;
	size_t	len;

	i = 1;
	while (args[i])
	{
		if (!is_valid_name(args[i]))
		{
			i++;
			continue ;
		}
		len = ft_strlen(args[i]);
		if (ft_strncmp(env_entry, args[i], len) == 0 && env_entry[len] == '=')
			return (1);
		i++;
	}
	return (0);
}

int	unset_remove_vars(char **args, char ***envcp)
{
	int		j;
	int		k;
	int		size;
	char	**new_env;

	size = 0;
	while ((*envcp)[size])
		size++;
	new_env = malloc(sizeof(char *) * (size + 1));
	if (!new_env)
		return (1);
	j = 0;
	k = 0;
	while (j < size)
	{
		if (should_unset_var((*envcp)[j], args))
			free((*envcp)[j]);
		else
			new_env[k++] = (*envcp)[j];
		j++;
	}
	new_env[k] = NULL;
	free(*envcp);
	*envcp = new_env;
	return (0);
}

int	unset_check_names(char **args)
{
	int	i;
	int	status;

	i = 1;
	status = 0;
	while (args[i])
	{
		if (!is_valid_name(args[i]))
		{
			ft_putstr_fd("minishell: unset: `", STDOUT_FILENO);
			ft_putstr_fd(args[i], STDOUT_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDOUT_FILENO);
			status = 1;
		}
		i++;
	}
	return (status);
}

int	unset_check_errors(char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (args[i][0] == '-' && args[i][1] != '\0')
		{
			ft_putstr_fd("minishell: ", STDOUT_FILENO);
			ft_putstr_fd(args[i], STDOUT_FILENO);
			ft_putstr_fd(": invalid option\n", STDOUT_FILENO);
			g_exit_status = 2;
			return (g_exit_status);
		}
		if (strchr(args[i], '!'))
		{
			ft_putstr_fd("Minishell: unset: `", STDOUT_FILENO);
			ft_putstr_fd(args[i], STDOUT_FILENO);
			ft_putstr_fd("': event not found\n", STDOUT_FILENO);
			g_exit_status = 1;
			return (g_exit_status);
		}
		i++;
	}
	return (0);
}

int	ft_unset(char **args, char ***envcp)
{
	int	exit_status;

	if (!args[1])
		return (0);
	exit_status = unset_check_errors(args);
	if (exit_status != 0)
		return (exit_status);
	exit_status = unset_check_names(args);
	unset_remove_vars(args, envcp);
	return (exit_status);
}
