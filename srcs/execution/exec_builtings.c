/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtings.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oceanepique <oceanepique@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 11:29:59 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/06 10:12:16 by oceanepique      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_exit_builtin(char **args, char ***envcp, t_com_list *cmd)
{
	ft_freeenvp(*envcp);
	ft_exit(args, 1, cmd);
	return (0);
}

static int	handle_export(char **args, char ***envcp)
{
	if (!args[1])
		return (ft_env(*envcp), 0);
	if (check_events(args[1]) == 0)
		return (ft_export(args, envcp));
	return (1);
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

int	exec_builting(char **args, char ***envcp, t_com_list *cmd)
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

// int	exec_builting(char **args, char ***envcp, t_com_list *cmd)
// {
// 	if (ft_strcmp(args[0], "exit") == 0)
// 	{
// 		ft_freeenvp(*envcp);
// 		return (ft_exit(args, 1, cmd), 0);
// 	}
// 	else if (ft_strcmp(args[0], "cd") == 0)
// 		return (ft_cd(args, envcp));
// 	else if (ft_strcmp(args[0], "pwd") == 0)
// 		return (ft_pwd(args, envcp));
// 	else if (ft_strcmp(args[0], "echo") == 0)
// 		return (ft_echo(args, envcp), 0);
// 	else if (ft_strcmp(args[0], "export") == 0)
// 	{
// 		if (!args[1])
// 			return (ft_env(*envcp), 0);
// 		else
// 		{
// 			if (check_events(args[1]) == 0)
// 				return (ft_export(args, envcp));
// 			else
// 				return (1);
// 		}
// 	}
// 	else if (ft_strcmp(args[0], "env") == 0)
// 	{
// 		if (args[1] && args[1][0] == '-')
// 		{
// 			printf("bash: env: -%c: invalid option\n", args[1][1]);
// 			g_exit_status = 2;
// 			return (g_exit_status);
// 		}
// 		else
// 			return (ft_env(*envcp), 0);
// 	}
// 	else if (ft_strcmp(args[0], "unset") == 0)
// 	{
// 		if (!args[1])
// 			return (0);
// 		if (check_events(args[1]) == 0)
// 			return (ft_unset(args, envcp));
// 		else
// 			return (1);
// 	}
// 	return (g_exit_status);
// }