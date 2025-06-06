/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 16:03:54 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/06 16:19:45 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*expand_clean_word(char *word, char **envcp)
{
	char	*expanded;
	char	*cleaned;

	expanded = replace_all_variables(word, envcp, 0);
	cleaned = remove_quotes_or_slash(expanded);
	free(expanded);
	return (cleaned);
}
