/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 12:00:38 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/05 12:34:13 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	limiter_is_quoted(const char *str)
{
	int	len;

	if (!str)
		return (0);
	len = ft_strlen(str);
	if (len < 2)
		return (0);
	if ((str[0] == '"' && str[len - 1] == '"')
		|| (str[0] == '\'' && str[len - 1] == '\''))
		return (1);
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
