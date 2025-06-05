/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredocs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 11:57:10 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/05 13:09:28 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// char	*handle_heredoc(char *limiter, char **envcp, int expand_var)
// {
// 	int		heredoc_fd;
// 	char	*filename;
// 	char	*line;
// 	char	*cleaned_limiter;
// 	char	*processed;

// 	filename = generate_tmp_filename();
// 	if (!filename)
// 		return (NULL);
// 	heredoc_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	if (heredoc_fd == -1)
// 		return(free(filename), NULL);
// 	while (1)
// 	{
// 		line = readline("> ");
// 		if (!line)
// 			break ;
// 		char *quoted = ft_strdup(limiter);
// 		if (!quoted)
// 			return (free(filename), NULL);
// 		cleaned_limiter = remove_quotes_or_slash(quoted);
// 		if (!cleaned_limiter)
// 			return (free(filename), free(quoted), NULL);
// 		free(quoted);
// 		if (ft_strcmp(line, cleaned_limiter) == 0)
// 		{
// 			free(cleaned_limiter);
// 			free(line);
// 			break ;
// 		}
// 		free (cleaned_limiter);
// 		processed = (expand_var) 
// 			? replace_all_variables(line, envcp, 1)
// 			: line;
// 		write(heredoc_fd, processed, ft_strlen(processed));
// 		write(heredoc_fd, "\n", 1);
// 		if (expand_var)
// 			free(processed);
// 	}
// 	if (close(heredoc_fd) == -1)
// 		perror("close heredoc_fd");
// 	return (filename);
// }

static char	*get_cleaned_limiter(char *limiter)
{
	char	*quoted;
	char	*cleaned;

	quoted = ft_strdup(limiter);
	if (!quoted)
		return (NULL);
	cleaned = remove_quotes_or_slash(quoted);
	free (quoted);
	return (cleaned);
}

static int	check_delimiter_match(char *line, char *cleaned_limiter)
{
	if (ft_strcmp(line, cleaned_limiter) == 0)
	{
		free(line);
		free(cleaned_limiter);
		return (1);
	}
	return (0);
}

static int	wr_heredoc_line(int fd, char *line, char **envcp, int expand_var)
{
	char	*processed;

	processed = line;
	if (expand_var)
	{
		processed = replace_all_variables(line, envcp, 1);
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
	char	*cleaned_limiter;

	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		cleaned_limiter = get_cleaned_limiter(limiter);
		if (!cleaned_limiter)
			return (free(line), -1);
		if (check_delimiter_match(line, cleaned_limiter))
			break ;
		if (wr_heredoc_line(fd, line, envcp, expand_var) < 0)
			return (free(line), free(cleaned_limiter), -1);
		free(line);
		free(cleaned_limiter);
	}
	return (0);
}

char	*handle_heredoc(char *limiter, char **envcp, int expand_var)
{
	char	*filename;
	int		heredoc_fd;

	filename = generate_tmp_filename();
	if (!filename)
		return (NULL);
	heredoc_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (heredoc_fd == -1)
		return (free(filename), NULL);
	if (heredoc_loop(heredoc_fd, limiter, envcp, expand_var) < 0)
		return (free(filename), NULL);
	if (close(heredoc_fd) == -1)
		perror("close heredoc_fd");
	return (filename);
}
