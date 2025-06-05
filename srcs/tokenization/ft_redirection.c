#include "../includes/minishell.h"

int	ft_redirect_in(char *file, int *fd_in)
{
	int	fd;

	fd = open_file_cmd(file);
	if (fd < 0)
		return (perror(file), -1);
	if (fd_in)
		*fd_in = dup(STDIN_FILENO);
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

int	ft_redirection_err(t_com_list *cmd, int *fd_err)
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

int	ft_redirection(t_com_list *cmd, int *fd_in, int *fd_out, int *fd_err)
{
	t_file_list	*tmp;
	int			fd;

	if (cmd->heredoc_fd > 0)
	{
		if (fd_in)
			*fd_in = dup(STDIN_FILENO);
		dup2(cmd->heredoc_fd, STDIN_FILENO);
		close(cmd->heredoc_fd);
	}
	if (cmd->infile && ft_redirect_in(cmd->infile, fd_in) < 0)
		return (-1);
	if (cmd->outfile && ft_redirect_out(cmd->outfile,
			cmd->flag_out, fd_out) < 0)
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

void	restore_redirections(int mem_fd_in, int mem_fd_out, int mem_fd_err)
{
	if (mem_fd_in != -1)
	{
		dup2(mem_fd_in, STDIN_FILENO);
		close(mem_fd_in);
	}
	if (mem_fd_out != -1)
	{
		dup2(mem_fd_out, STDOUT_FILENO);
		close(mem_fd_out);
	}
	if (mem_fd_err != -1)
	{
		dup2(mem_fd_err, STDERR_FILENO);
		close(mem_fd_err);
	}
}
