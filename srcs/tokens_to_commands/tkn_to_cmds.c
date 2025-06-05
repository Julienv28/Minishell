/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tkn_to_cmds.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 13:09:09 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/05 15:44:14 by juvitry          ###   ########.fr       */
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

static void	process_token(t_parser_context *ctx)
{
	int	type;

	type = ctx->current_token->type;
	if (type == CMD)
		handle_cmd_token(ctx);
	else if (type == ARG)
		handle_arg_token(ctx);
	else if (type == PIPE)
		handle_pipe_token(ctx);
	else if (type == TRUNC || type == APPEND
		|| type == INPUT || type == HEREDOC)
		handle_redir_token(ctx);
	else
		ctx->current_token = ctx->current_token->next;
}

t_com_list	*tokens_to_cmds(t_token *tokens, char **envcp)
{
	t_parser_context	*ctx;
	t_com_list			*result;

	ctx = init_parset_ctx(tokens, envcp);
	if (!ctx)
		return (NULL);
	while (ctx->current_token)
		process_token(ctx);
	if ((ctx->pending_outfile || ctx->pending_infile) && !ctx->current_cmd)
		finalize_pending_redirs(ctx);
	result = ctx->cmd_list;
	free(ctx);
	return (result);
}

// t_com_list	*tokens_to_cmds(t_token *tokens, char **envcp)
// {
// 	int					type;
// 	t_parser_context	*ctx;
// 	t_com_list			*result;

// 	ctx = init_parset_ctx(tokens, envcp);
// 	if (!ctx)
// 		return (NULL);
// 	while (ctx->current_token)
// 	{
// 		type = ctx->current_token->type;
// 		if (type == CMD)
// 			handle_cmd_token(ctx);
// 		else if (type == ARG)
// 			handle_arg_token(ctx);
// 		else if (type == PIPE)
// 			handle_pipe_token(ctx);
// 		else if (type == TRUNC || type == APPEND
// 			|| type == INPUT || type == HEREDOC)
// 			handle_redir_token(ctx);
// 		else
// 			ctx->current_token = ctx->current_token->next;
// 	}
// 	if ((ctx->pending_outfile || ctx->pending_infile) && !ctx->current_cmd)
// 		finalize_pending_redirs(ctx);
// 	result = ctx->cmd_list;
// 	return (free(ctx), result);
// }
