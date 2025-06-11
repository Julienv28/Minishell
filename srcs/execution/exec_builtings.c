/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtings.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oceanepique <oceanepique@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 11:29:59 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/11 17:08:52 by oceanepique      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_exit_builtin(char **args, char ***envcp, t_com *cmd)
{
	ft_freeenvp(*envcp);
	ft_exit(args, 1, cmd);
	return (0);
}

int	handle_export(char **args, char ***envcp)
{
	int	exit_status;

	if (!args[1])
		return (export_no_args(*envcp));
	if (check_events(args[1]) == 1)
		return (1);
	exit_status = handle_initial_errors(args);
	process_valid(args, envcp, &exit_status);
	return (exit_status);
}

static int	handle_env(char **args, char ***envcp)
{
	if (args[1] && args[1][0] == '-')
	{
		printf("minishell: env: -%c: invalid option\n", args[1][1]);
		g_exit_status = 2;
		return (g_exit_status);
	}
	return (ft_env(*envcp), 0);
}

static int	handle_unset(char **args, char ***envcp)
{
	if (!args[1])
		return (0);
	if (check_events(args[1]) == 0)
		return (ft_unset(args, envcp));
	return (1);
}

int	exec_builting(char **args, char ***envcp, t_com *cmd)
{
	if (ft_strcmp(args[0], "exit") == 0)
		return (handle_exit_builtin(args, envcp, cmd));
	else if (ft_strcmp(args[0], "cd") == 0)
		return (ft_cd(args, envcp));
	else if (ft_strcmp(args[0], "pwd") == 0)
		return (ft_pwd(args, envcp));
	else if (ft_strcmp(args[0], "echo") == 0)
		return (ft_echo(args, envcp), 0);
	else if (ft_strcmp(args[0], "export") == 0)
		return (handle_export(args, envcp));
	else if (ft_strcmp(args[0], "env") == 0)
		return (handle_env(args, envcp));
	else if (ft_strcmp(args[0], "unset") == 0)
		return (handle_unset(args, envcp));
	return (g_exit_status);
}
