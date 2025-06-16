/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 16:03:54 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/16 13:56:15 by juvitry          ###   ########.fr       */
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
