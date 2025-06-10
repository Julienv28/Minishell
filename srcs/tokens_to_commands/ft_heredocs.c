/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredocs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opique <opique@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 11:57:10 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/10 09:28:27 by opique           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	assign_heredoc_to_ctx(t_parser_context *ctx, char *heredoc_name)
{
	if (!ctx->current_cmd)
	{
		ctx->pending_infile = ft_strdup(heredoc_name);
		if (!ctx->pending_infile)
			return (-1);
	}
	else
	{
		free(ctx->current_cmd->infile);
		ctx->current_cmd->infile = ft_strdup(heredoc_name);
		if (!ctx->current_cmd->infile)
			return (-1);
	}
	return (0);
}

static int	wr_heredoc_line(int fd, char *line, char **envcp, int expand_var)
{
	char	*processed;

	processed = line;
	if (expand_var)
	{
		processed = replace_all_variables(line, envcp, 1, expand_var);
		if (!processed)
			return (-1);
	}
	write(fd, processed, ft_strlen(processed));
	write(fd, "\n", 1);
	if (expand_var)
		free(processed);
	return (0);
}

static int	heredoc_loop(int fd, char *limiter, char **envcp, int expand_var)
{
	char	*line;
	int		status;

	signal(SIGINT, heredoc_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		line = readline("heredoc> ");
		if (!line || g_exit_status == 130)
		{
			status = handle_heredoc_interrupt(line, !line);
			if (status != 0)
				return (status);
			continue ;
		}
		if (ft_strcmp(line, limiter) == 0)
			return (free(line), signal(SIGINT, handler_sigint), 0);
		if (wr_heredoc_line(fd, line, envcp, expand_var) < 0)
			return (free(line), -1);
		free(line);
	}
}

char	*handle_heredoc(char *limiter, char **envcp, int expand_var)
{
	char	*filename;
	int		heredoc_fd;
	int		loop_ret;
	int		saved_stdin;

	filename = generate_tmp_filename();
	if (!filename)
		return (free(limiter), NULL);
	heredoc_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (heredoc_fd == -1)
		return (free(filename), free(limiter), NULL);
	saved_stdin = dup(STDIN_FILENO);
	loop_ret = heredoc_loop(heredoc_fd, limiter, envcp, expand_var);
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdin);
	if (loop_ret == 1)
		return (heredoc_cleanup(filename, limiter, heredoc_fd, 1));
	else if (loop_ret < 0)
		return (heredoc_cleanup(filename, limiter, heredoc_fd, 0));
	if (close(heredoc_fd) == -1)
		perror("close heredoc_fd");
	return (filename);
}
