/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_pipes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opique <opique@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 16:07:39 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/16 16:21:45 by opique           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	reset_signals(void)
{
	signal(SIGINT, handler_sigint);
	signal(SIGQUIT, SIG_IGN);
}

void	set_signals_child(void)
{
	signal(SIGTSTP, SIG_IGN);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	set_signals_parent(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	handle_pipes(char **args, t_com *cmds, t_msh *msh)
{
	if (!args || !args[0] || args[0][0] == '\0')
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		if (args && args[0])
			ft_putstr_fd(args[0], STDERR_FILENO);
		else
			ft_putstr_fd("", STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		rl_clear_history();
		ft_freeenvp(msh->envcp);
		cleanup_and_exit(127, cmds);
	}
	if (is_builting(args[0]) && ft_strcmp(args[0], "exit") == 0)
	{
		ft_freeenvp(msh->envcp);
		fake_exit_builtin(args, cmds);
	}
	else if (is_builting(args[0]))
		msh->ex_status = exec_builting(args, msh, cmds);
	else
		exec_cmd(args, msh);
}

void	wait_children(pid_t last_pid, t_msh *msh)
{
	int		status;
	pid_t	pid;

	pid = wait(&status);
	while (pid > 0)
	{
		if (pid == last_pid)
		{
			if (WIFEXITED(status))
				msh->ex_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				msh->ex_status = 128 + WTERMSIG(status);
		}
		pid = wait(&status);
	}
	if (WTERMSIG(status) == SIGQUIT)
		write(1, "Quit (core dumped)\n", 20);
}
