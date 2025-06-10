/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 12:00:38 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/10 16:06:45 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	limiter_is_quoted(const char *str)
{
	if (!str)
		return (0);
	while (*str)
	{
		if (*str == '\'' || *str == '"')
			return (1);
		str++;
	}
	return (0);
}

char	*generate_tmp_filename(void)
{
	static int	counter = 0;
	t_filename	filename;

	filename.prefix = ft_strdup("/tmp/.heredoc_");
	filename.pid_str = ft_itoa(getpid());
	filename.count_str = ft_itoa(counter++);
	if (!filename.prefix || !filename.pid_str || !filename.count_str)
	{
		free(filename.prefix);
		free(filename.pid_str);
		free(filename.count_str);
		return (NULL);
	}
	filename.tmp = ft_strjoin(filename.prefix, filename.pid_str);
	filename.final = ft_strjoin(filename.tmp, filename.count_str);
	free(filename.prefix);
	free(filename.pid_str);
	free(filename.count_str);
	free (filename.tmp);
	return (filename.final);
}

char	*heredoc_cleanup(char *filename, char *limiter, int fd, int status)
{
	close(fd);
	free(limiter);
	free(filename);
	if (status == 130)
		g_exit_status = 130;
	return (NULL);
}

int	print_heredoc_eof_error(void)
{
	ft_putstr_fd("minishell: unexpected EOF while looking for \
		matching ''\n", STDERR_FILENO);
	ft_putstr_fd("syntax error: unexpected end of \
		file\n", STDERR_FILENO);
	return (-1);
}

int	handle_heredoc_interrupt(char *line, int eof)
{
	if (line)
		free(line);
	if (g_exit_status == 130)
		return (1);
	if (eof)
		return (print_heredoc_eof_error());
	return (0);
}
