/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processing_tokens.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oceanepique <oceanepique@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:56:26 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/16 19:45:57 by oceanepique      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	process_pipe(t_parse_ctx *ctx)
{
	if (ctx->str[ctx->i] == '|')
	{
		if (check_pipe(ctx->str, ctx->i) == -1)
			return (ctx->msh->ex_status = 2, -1);
		add_token(&ctx->tokens, "|", PIPE, 0);
		ctx->i++;
		ctx->is_cmd = 1;
		return (1);
	}
	return (0);
}

int	process_special_chars(char *str, int i)
{
	if (str[i] == '&' || str[i] == ':'
		|| str[i] == '!' || str[i] == '#' || str[i] == ';')
	{
		if (check_input(str, i) == -1)
			return (-1);
	}
	return (0);
}

int	process_redir(t_parse_ctx *ctx)
{
	int	redirection_status;

	redirection_status = handle_redir(ctx);
	if (redirection_status == -1)
		return (-1);
	return (redirection_status);
}

int	process_word(t_parse_ctx *ctx)
{
	int	ret;

	ret = handle_word(ctx);
	if (ret == -1)
		return (-1);
	if (ret == 1)
		return (1);
	return (0);
}
