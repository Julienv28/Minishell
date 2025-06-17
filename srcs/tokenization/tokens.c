/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 16:46:03 by opique            #+#    #+#             */
/*   Updated: 2025/06/17 11:16:27 by juvitry          ###   ########.fr       */
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

static void	reinit_ctx(t_parse_ctx *ctx, t_msh *msh, char *str)
{
	ctx->i = 0;
	ctx->is_cmd = 1;
	ctx->tokens = NULL;
	ctx->msh = msh;
	ctx->str = str;
}

static t_tkn	*process_token_loop(char *str, t_msh *msh)
{
	t_parse_ctx	ctx;
	int			ret;

	reinit_ctx(&ctx, msh, str);
	while (ctx.str[ctx.i] && skip_spaces(ctx.str, &ctx.i) != -1)
	{
		ret = process_pipe(&ctx);
		if (ret == -1 || process_special_chars(ctx.str, ctx.i) == -1)
			return (free_tokens(ctx.tokens), NULL);
		if (ret == 1)
			continue ;
		ret = process_redir(&ctx);
		if (ret == -1)
			return (free_tokens(ctx.tokens), NULL);
		if (ret == 1)
			continue ;
		ret = process_word(&ctx);
		if (ret == -1 || ret == 1)
			return (free_tokens(ctx.tokens), NULL);
		else if (ret == -2)
			return (ctx.msh->ex_status = 1, free_tokens(ctx.tokens), NULL);
	}
	return (ctx.tokens);
}

t_tkn	*create_tokens(char **str, t_msh *msh)
{
	t_tkn	*tokens;

	tokens = process_token_loop(*str, msh);
	return (tokens);
}
