/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 16:03:54 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/17 10:46:58 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*expand_clean_word(char *word, t_msh *msh)
{
	char	*expanded;
	char	*cleaned;

	expanded = replace_var(word, msh, 0, 1);
	cleaned = remove_quotes_or_slash(expanded);
	return (cleaned);
}

void	restore_and_close_fd(int *fd, int std_fd)
{
	if (fd && *fd >= 0)
	{
		dup2(*fd, std_fd);
		close(*fd);
		*fd = -1;
	}
}

void	restor_redir(int mem_fd_in, int mem_fd_out, int mem_fd_err)
{
	restore_and_close_fd(&mem_fd_in, STDIN_FILENO);
	restore_and_close_fd(&mem_fd_out, STDOUT_FILENO);
	restore_and_close_fd(&mem_fd_err, STDERR_FILENO);
}

char	*get_input(t_msh *msh)
{
	char	*input;

	set_signal_action();
	input = readline(GREEN "minishell$ " RESET);
	if (g_sig_status != 0)
	{
		if (g_sig_status == 130)
			msh->ex_status = 130;
		else if (g_sig_status == 131)
			msh->ex_status = 131;
		g_sig_status = 0;
	}
	return (input);
}
