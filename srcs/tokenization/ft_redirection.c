/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirection.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opique <opique@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 16:35:46 by opique            #+#    #+#             */
/*   Updated: 2025/06/16 14:18:03 by opique           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_redirect_in(char *file, int *fd_in)
{
	int	fd;

	fd = open_file_cmd(file);
	if (fd < 0)
		return (perror(file), -1);
	if (fd_in)
	{
		*fd_in = dup(STDIN_FILENO);
		if (*fd_in < 0)
			return (perror("dup"), close(fd), -1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int	ft_redirect_out(char *file, int flag, int *fd_out)
{
	int	fd;

	fd = open_outfile(file, flag);
	if (fd < 0)
		return (perror(file), -1);
	if (fd_out)
		*fd_out = dup(STDOUT_FILENO);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	ft_redirection_err(t_com *cmd, int *fd_err)
{
	int	fd;

	if (!cmd->errfile)
		return (0);
	fd = open_errfile(cmd->errfile);
	if (fd < 0)
		return (perror(cmd->errfile), -1);
	if (fd_err)
		*fd_err = dup(STDERR_FILENO);
	dup2(fd, STDERR_FILENO);
	close(fd);
	return (0);
}

static int	ft_redir_in(t_com *cmd, int *fd_in)
{
	if (cmd->heredoc_fd > 0)
	{
		if (fd_in)
		{
			*fd_in = dup(STDIN_FILENO);
			if (*fd_in < 0)
				return (perror("dup"), close(cmd->heredoc_fd), -1);
		}
		dup2(cmd->heredoc_fd, STDIN_FILENO);
		close(cmd->heredoc_fd);
	}
	if (cmd->infile && ft_redirect_in(cmd->infile, fd_in) < 0)
		return (-1);
	return (0);
}

int	ft_redir(t_com *cmd, int *fd_in, int *fd_out, int *fd_err)
{
	t_file_list	*tmp;
	int			fd;

	if (ft_redir_in(cmd, fd_in) < 0)
		return (-1);
	if (cmd->outfile
		&& ft_redirect_out(cmd->outfile, cmd->flag_out, fd_out) < 0)
		return (-1);
	tmp = cmd->all_outfilles;
	while (tmp)
	{
		fd = open_outfile(tmp->filename, tmp->flag);
		if (fd < 0)
			return (perror(tmp->filename), -1);
		close(fd);
		tmp = tmp->next;
	}
	return (ft_redirection_err(cmd, fd_err));
}
