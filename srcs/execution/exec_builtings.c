/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtings.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 11:29:59 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/16 14:45:24 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_exit_builtin(char **args, t_msh *msh, t_com *cmd)
{
	ft_freeenvp(msh->envcp);
	ft_exit(args, cmd, msh);
	return (0);
}

int	handle_export(char **args, t_msh *msh)
{
	int	exit_status;

	if (!args[1])
		return (export_no_args(msh->envcp));
	if (check_events(args[1]) == 1)
		return (1);
	exit_status = handle_initial_errors(args);
	process_valid(args, &exit_status, msh);
	return (exit_status);
}

static int	handle_env(char **args, t_msh *msh)
{
	if (args[1] && args[1][0] == '-')
	{
		ft_putstr_fd("minishell: env -", STDERR_FILENO);
		ft_putchar_fd(args[1][1], STDERR_FILENO);
		ft_putstr_fd(": invalid option\n", STDERR_FILENO);
	  msh->ex_status = 2;
		return (msh->ex_status);
	}
	return (ft_env(msh->envcp), 0);
}

static int	handle_unset(char **args, t_msh *msh)
{
	if (!args[1])
		return (0);
	if (check_events(args[1]) == 0)
		return (ft_unset(args, msh));
	return (1);
}

int	exec_builting(char **args, t_msh *msh, t_com *cmd)
{
	if (ft_strcmp(args[0], "exit") == 0)
		return (handle_exit_builtin(args, msh, cmd));
	else if (ft_strcmp(args[0], "cd") == 0)
		return (ft_cd(args, msh));
	else if (ft_strcmp(args[0], "pwd") == 0)
		return (ft_pwd(args, msh));
	else if (ft_strcmp(args[0], "echo") == 0)
		return (ft_echo(args, &(msh->envcp)), 0);
	else if (ft_strcmp(args[0], "export") == 0)
		return (handle_export(args, msh));
	else if (ft_strcmp(args[0], "env") == 0)
		return (handle_env(args, msh));
	else if (ft_strcmp(args[0], "unset") == 0)
		return (handle_unset(args, msh));
	return (msh->ex_status);
}
