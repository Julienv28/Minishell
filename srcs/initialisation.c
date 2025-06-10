/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialisation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opique <opique@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 16:27:35 by opique            #+#    #+#             */
/*   Updated: 2025/06/10 16:27:37 by opique           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	init_signals(void)
{
	signal(SIGTSTP, SIG_IGN);
	signal(SIGINT, handler_sigint);
	signal(SIGQUIT, SIG_IGN);
}

int	check_isatty(void)
{
	if (!isatty(0) || !isatty(1))
	{
		write(1, "you can't pipe a minishell\n", 27);
		return (0);
	}
	return (1);
}

void	init_cmds(t_com_list *command)
{
	command->outfile = NULL;
	command->infile = NULL;
	command->errfile = NULL;
	command->flag_in = 0;
	command->flag_out = 0;
}

void	init_redirs(t_redirs *fds)
{
	fds->in = -1;
	fds->out = -1;
	fds->err = -1;
}
