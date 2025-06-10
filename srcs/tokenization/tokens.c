/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opique <opique@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 16:46:03 by opique            #+#    #+#             */
/*   Updated: 2025/06/10 16:57:07 by opique           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	*add_token(t_token **head, char *str, int type, int is_quoted)
{
	t_token	*new;
	t_token	*tmp;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->value = ft_strdup(str);
	new->type = type;
	new->is_quoted = is_quoted;
	new->next = NULL;
	if (!*head)
		*head = new;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	return (new);
}

int	skip_spaces(char *str, int *i)
{
	while (str[*i] == ' ')
		(*i)++;
	if (!str[*i])
		return (-1);
	return (0);
}

t_token	*process_token_loop(char *str, char **envcp)
{
	int		i;
	int		tab[2];
	t_token	*tokens;

	i = 0;
	tab[0] = 1;
	tokens = NULL;
	while (str[i])
	{
		if (skip_spaces(str, &i) == -1)
			break ;
		tab[1] = process_pipe(str, &i, &tokens, &tab[0]);
		if (tab[1] == -1 || process_special_chars(str, i) == -1)
			return (free_tokens(tokens), NULL);
		if (tab[1] == 1)
			continue ;
		tab[1] = process_redirection(str, &i, &tokens, envcp);
		if (tab[1] == -1)
			return (free_tokens(tokens), NULL);
		if (tab[1] == 1)
			continue ;
		tab[1] = process_word(&str, &i, &tokens, &tab[0]);
		if (tab[1] == -1 || tab[1] == 1)
			return (free_tokens(tokens), NULL);
	}
	return (tokens);
}

t_token	*create_tokens(char **str, char **envcp)
{
	t_token	*tokens;

	tokens = process_token_loop(*str, envcp);
	return (tokens);
}
