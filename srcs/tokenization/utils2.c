/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 16:03:54 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/12 19:28:44 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*expand_clean_word(char *word, char **envcp)
{
	char	*expanded;
	char	*cleaned;

	expanded = replace_var(word, envcp, 0, 1);
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
