/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opique <opique@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 16:27:55 by opique            #+#    #+#             */
/*   Updated: 2025/06/12 10:26:42 by opique           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_exit_status = 0;

int	has_redirection(t_com *cmd)
{
	return (cmd->infile || cmd->outfile || cmd->errfile || cmd->heredoc_fd > 0);
}

int	handle_execution(t_com *cmd, char ***envcp, t_redirs *fds)
{
	int	error;

	error = 0;
	if (has_redirection(cmd))
		error = ft_redir(cmd, &fds->in, &fds->out, &fds->err);
	if (error)
		return (0);
	if (has_pipe(cmd))
	{
		exec_pipes(cmd, *envcp);
		restor_redir(fds->in, fds->out, fds->err);
		init_redirs(fds);
		return (1);
	}
	execute(cmd, envcp);
	if (has_redirection(cmd))
		restor_redir(fds->in, fds->out, fds->err);
	init_redirs(fds);
	return (0);
}

int	handle_empty_command(t_com *cmd, t_redirs *fds)
{
	int	error;

	if (cmd->cmd)
		return (0);
	if (!has_redirection(cmd))
		return (1);
	error = ft_redir(cmd, &fds->in, &fds->out, &fds->err);
	if (error != 0)
		g_exit_status = 1;
	restor_redir(fds->in, fds->out, fds->err);
	init_redirs(fds);
	return (1);
}

void	execute_commands(t_com *cmd, char ***envcp)
{
	t_com		*cur;
	t_redirs	fds;

	cur = cmd;
	init_redirs(&fds);
	while (cur)
	{
		if (handle_empty_command(cur, &fds))
		{
			cur = cur->next;
			continue ;
		}
		if (handle_execution(cur, envcp, &fds))
			break ;
		cur = cur->next;
	}
}

int	main(int ac, char **av, char **envp)
{
	char	**envcp;

	(void)ac;
	(void)av;
	if (!check_isatty())
		return (0);
	envcp = ft_env_dup(envp);
	init_signals();
	minishell_loop(&envcp);
	ft_freeenvp(envcp);
	rl_clear_history();
	return (g_exit_status);
}
