/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opique <opique@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 10:45:02 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/10 15:40:48 by opique           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	fake_exit_builtin(char **args, t_com_list *cmds)
{
	long long	exit_value;

	exit_value = 0;
	if (!args[1])
		cleanup_and_exit(0, cmds);
	if (!is_valid_numeric_argument(args[1]))
	{
		fprintf(stderr, "minishell: exit: %s: numeric argument required\n", \
				args[1]);
		cleanup_and_exit(255, cmds);
	}
	exit_value = ft_atoull(args[1]);
	exit((unsigned char)(exit_value));
}

static void	handle_exit(char **args, t_com_list *cmds, char ***envcp)
{
	ft_freeenvp(*envcp);
	rl_clear_history();
	ft_exit(args, 0, cmds);
}

static int	handle_empty(void)
{
	g_exit_status = 0;
	return (0);
}

int	execute(t_com_list *cmds, char ***envcp)
{
	char	**args;

	if (!cmds)
		return (-1);
	args = cmds->args;
	if (!args || !args[0] || args[0][0] == '\0')
		return (handle_empty());
	if (is_builting(args[0]) && ft_strcmp(args[0], "exit") == 0)
		handle_exit(args, cmds, envcp);
	else if (is_builting(args[0]))
		g_exit_status = exec_builting(args, envcp, cmds);
	else
		g_exit_status = exec_external(args, envcp);
	return (g_exit_status);
}
