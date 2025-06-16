/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oceanepique <oceanepique@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 09:31:19 by opique            #+#    #+#             */
/*   Updated: 2025/06/16 19:45:35 by oceanepique      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	process_redir_val(int type, char *word, t_parse_ctx *ctx)
{
	char	*final;
	int		is_quoted;
	char	*cleaned_limiter;
	char	*instru;

	final = NULL;
	is_quoted = limiter_is_quoted(word);
	if (type == HEREDOC && is_quoted)
        return (add_token(&ctx->tokens, word, ARG, 1), free(word), 1);
	else if (type == HEREDOC)
	{
		cleaned_limiter = remove_quotes_or_slash(word);
		if (!cleaned_limiter)
			return (-1);
		instru = ft_strdup(cleaned_limiter);
		if (!add_token(&ctx->tokens, instru, ARG, 0))
			return (free(cleaned_limiter), free(word), -1);
        return (free(instru), free(cleaned_limiter), 1);
	}
	final = expand_clean_word(word, ctx->msh);
	if (!final)
		return (free(word), -1);
	free(word);
	add_token(&ctx->tokens, final, ARG, 0);
	return (free(final), 1);
}

char	*add_symbol(int type)
{
	if (type == HEREDOC)
		return (ft_strdup("<<"));
	else if (type == APPEND)
		return (ft_strdup(">>"));
	else if (type == INPUT)
		return (ft_strdup("<"));
	else if (type == TRUNC)
		return (ft_strdup(">"));
	else if (type == ERR_REDIR)
		return (ft_strdup("2>"));
	return (NULL);
}

char	*extract_redir_word(char *str, int *i)
{
	int	start;

	while (str[*i] == ' ')
		(*i)++;
	start = *i;
	if (!str[*i] || str[*i] == '<' || str[*i] == '>' || str[*i] == '|')
		return (NULL);
	while (str[*i] && str[*i] != ' ' && str[*i] != '|'
		&& str[*i] != '<' && str[*i] != '>')
		(*i)++;
	if (*i == start)
		return (NULL);
	return (ft_strndup(str + start, *i - start));
}

int	parse_redirection(char *str, int *i)
{
	if (str[*i] == '2' && str[*i + 1] == '>' && str[*i + 2] == '>')
		return ((*i) += 3, ERR_REDIR);
	else if (str[*i] == '2' && str[*i + 1] == '>')
		return ((*i) += 2, ERR_REDIR);
	if (str[*i] == '<' && str[*i + 1] == '<')
		return ((*i) += 2, HEREDOC);
	else if (str[*i] == '>' && str[*i + 1] == '>')
		return ((*i) += 2, APPEND);
	else if (str[*i] == '<')
		return ((*i)++, INPUT);
	else if (str[*i] == '>')
		return ((*i)++, TRUNC);
	return (0);
}

int	handle_redir(t_parse_ctx *ctx)
{
	int		type;
	char	*symbol;
	char	*word;

	symbol = NULL;
	word = NULL;
	type = parse_redirection(ctx->str, &ctx->i);
	if (!type)
		return (0);
	if (check_redirection(ctx->str, &ctx->i) == -1)
		return (ctx->msh->ex_status = 2, -1);
	word = extract_redir_word(ctx->str, &ctx->i);
	if (!word)
	{
		ctx->msh->ex_status = 1;
		ft_putstr_fd("minishell: syntax error near redirection\n", STDERR_FILENO);
		return (-1);
	}
	symbol = add_symbol(type);
	if (!symbol)
		return (free(word), -1);
	add_token(&ctx->tokens, symbol, type, 0);
	free(symbol);
	return (process_redir_val(type, word, ctx));
}
