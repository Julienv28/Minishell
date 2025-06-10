/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tkn_to_cmds.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opique <opique@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 13:09:09 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/10 15:43:34 by opique           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static t_parser_context	*init_parset_ctx(t_token *tokens, char **envcp)
{
	t_parser_context	*ctx;

	ctx = ft_calloc(1, sizeof(t_parser_context));
	if (!ctx)
		return (NULL);
	ctx->cmd_list = NULL;
	ctx->current_cmd = NULL;
	ctx->pending_flag_in = -1;
	ctx->pending_flag_out = -1;
	ctx->current_token = tokens;
	ctx->envcp = envcp;
	return (ctx);
}

static int process_token(t_parser_context *ctx)
{
	int	type;
	int	ret;

	type = ctx->current_token->type;
	if (type == CMD)
		return (handle_cmd_token(ctx), 0);
	else if (type == ARG)
		return (handle_arg_token(ctx), 0);
	else if (type == PIPE)
		return (handle_pipe_token(ctx), 0);
	else if (type == TRUNC || type == APPEND
		|| type == INPUT || type == HEREDOC)
	{
		ret = handle_redir_token(ctx);
		if (ret != 0)
			return (ret);
		return (0);
	}
	else
	{
		ctx->current_token = ctx->current_token->next;
		return (0);
	}
}

t_com_list	*tokens_to_cmds(t_token *tokens, char **envcp)
{
	t_parser_context	*ctx;
	t_com_list			*result;
	int					ret;

	ctx = init_parset_ctx(tokens, envcp);
	if (!ctx)
		return (NULL);
	while (ctx->current_token)
	{
		ret = process_token(ctx);
		if (ret != 0)
			break ;
	}
	if ((ctx->pending_outfile || ctx->pending_infile) && !ctx->current_cmd)
		finalize_pending_redirs(ctx);
	if (ret == 1)
	{
		free_cmd(ctx->cmd_list);
		free(ctx);
		return (NULL);
	}
	result = ctx->cmd_list;
	free(ctx);
	return (result);
}
