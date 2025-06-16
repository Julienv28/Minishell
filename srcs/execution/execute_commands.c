/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 10:45:02 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/16 14:43:27 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	fake_exit_builtin(char **args, t_com *cmds)
{
	long long	exit_value;

	exit_value = 0;
	if (!args[1])
		cleanup_and_exit(0, cmds);
	if (!is_valid_numeric_argument(args[1]))
	{
		ft_putstr_fd("minishell: exit:", STDERR_FILENO);
		ft_putstr_fd(args[1], STDERR_FILENO);
		ft_putstr_fd("numeric argument required\n", STDERR_FILENO);
		cleanup_and_exit(255, cmds);
	}
	exit_value = ft_atoull(args[1]);
	exit((unsigned char)(exit_value));
}

static void	handle_exit(char **args, t_com *cmds, t_msh *msh)
{
	ft_freeenvp(msh->envcp);
	rl_clear_history();
	ft_exit(args, 0, cmds, msh);
}

static int	handle_empty(void)
{
	g_sig_status = 0;
	return (0);
}

int	execute(t_com *cmds, t_msh *msh)
{
	char	**args;

	if (!cmds)
		return (-1);
	args = cmds->args;
	if (!args || !args[0] || args[0][0] == '\0')
		return (handle_empty());
	if (is_builting(args[0]) && ft_strcmp(args[0], "exit") == 0)
		handle_exit(args, cmds, msh);
	else if (is_builting(args[0]))
		msh->ex_status = exec_builting(args, msh, cmds);
	else
		msh->ex_status = exec_external(args, msh);
	return (msh->ex_status);
}
