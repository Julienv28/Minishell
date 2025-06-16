/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opique <opique@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 16:46:03 by opique            #+#    #+#             */
/*   Updated: 2025/06/16 16:12:57 by opique           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_tkn	*add_token(t_tkn **head, char *s, int type, int is_quote)
{
	t_tkn	*new;
	t_tkn	*tmp;

	new = malloc(sizeof(t_tkn));
	if (!new)
		return (NULL);
	new->value = ft_strdup(s);
	new->type = type;
	new->is_quoted = is_quote;
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
	while (str[*i] == ' ' || str[*i] == '\t')
		(*i)++;
	if (!str[*i])
		return (-1);
	return (0);
}

static t_tkn	*process_token_loop(char *str, t_msh *msh)
{
	int		i;
	int		tab[2];
	t_tkn	*tokens;

	i = 0;
	tab[0] = 1;
	tokens = NULL;
	while (str[i] && skip_spaces(str, &i) != -1)
	{
		tab[1] = process_pipe(str, &i, &tokens, &tab[0], msh);
		if (tab[1] == -1 || process_special_chars(str, i) == -1)
			return (free_tokens(tokens), NULL);
		if (tab[1] == 1)
			continue ;
		tab[1] = process_redir(str, &i, &tokens, msh);
		if (tab[1] == -1)
			return (free_tokens(tokens), NULL);
		if (tab[1] == 1)
			continue ;
		tab[1] = process_word(&str, &i, &tokens, &tab[0]);
		if (tab[1] == -1 || tab[1] == 1)
			return (free_tokens(tokens), NULL);
		else if (tab[1] == -2)
			return (msh->ex_status = 1, free_tokens(tokens), NULL);
	}
	return (tokens);
}

t_tkn	*create_tokens(char **str, t_msh *msh)
{
	t_tkn	*tokens;

	tokens = process_token_loop(*str, msh);
	return (tokens);
}
