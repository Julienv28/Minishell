/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 16:27:55 by opique            #+#    #+#             */
/*   Updated: 2025/06/16 15:54:21 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_sig_status = 0;

int	has_redirection(t_com *cmd)
{
	return (cmd->infile || cmd->outfile || cmd->errfile || cmd->heredoc_fd > 0);
}

int	handle_execution(t_com *cmd, t_msh *msh, t_redirs *fds)
{
	int	error;

	error = 0;
	if (has_redirection(cmd))
		error = ft_redir(cmd, &fds->in, &fds->out, &fds->err);
	if (error)
		return (0);
	if (has_pipe(cmd))
	{
		exec_pipes(cmd, msh);
		restor_redir(fds->in, fds->out, fds->err);
		init_redirs(fds);
		return (1);
	}
	execute(cmd, msh);
	if (has_redirection(cmd))
		restor_redir(fds->in, fds->out, fds->err);
	init_redirs(fds);
	return (0);
}

int	handle_empty_command(t_com *com, t_redirs *fds, t_msh *msh)
{
	int	error;

	if (com->cmd)
		return (0);
	if (!has_redirection(com))
		return (1);
	error = ft_redir(com, &fds->in, &fds->out, &fds->err);
	if (error != 0)
		msh->ex_status = 1;
	restor_redir(fds->in, fds->out, fds->err);
	init_redirs(fds);
	return (1);
}

void	execute_commands(t_msh *msh)
{
	t_com		*cur;
	t_redirs	fds;

	cur = msh->com;
	init_redirs(&fds);
	while (cur)
	{
		if (handle_empty_command(cur, &fds, msh))
		{
			cur = cur->next;
			continue ;
		}
		if (handle_execution(cur, msh, &fds))
			break ;
		cur = cur->next;
	}
}

int	main(int ac, char **av, char **envp)
{
	t_msh	*msh;

	(void)ac;
	(void)av;
	msh = ft_calloc(1, sizeof(t_msh));
	if (!msh)
		return (0);
	if (!check_isatty())
		return (0);
	msh->envcp = ft_env_dup(envp);
	if (!msh->envcp)
		return (1);
	init_signals();
	minishell_loop(msh);
	ft_freeenvp(msh->envcp);
	rl_clear_history();
	return (msh->ex_status);
}
