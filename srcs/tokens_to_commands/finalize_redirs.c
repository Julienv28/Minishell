/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finalize_redirs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 15:32:18 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/05 15:36:01 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	reinit_pending(t_parser_context *ctx)
{
	ctx->pending_outfile = NULL;
	ctx->pending_infile = NULL;
	ctx->pending_errfile = NULL;
	ctx->pending_flag_out = -1;
	ctx->pending_flag_in = -1;
	ctx->pending_all_outfiles = NULL;
}

static void	assign_pending_redirs(t_com_list *cmd, t_parser_context *ctx)
{
	cmd->heredoc_fd = -1;
	if (ctx->pending_outfile)
	{
		cmd->outfile = ctx->pending_outfile;
		cmd->flag_out = ctx->pending_flag_out;
		cmd->all_outfilles = ctx->pending_all_outfiles;
	}
	if (ctx->pending_infile)
	{
		cmd->infile = ctx->pending_infile;
		cmd->flag_in = ctx->pending_flag_in;
	}
	if (ctx->pending_errfile)
		cmd->errfile = ctx->pending_errfile;
}

t_com_list	*finalize_pending_redirs(t_parser_context *ctx)
{
	t_com_list	*new_cmd;

	if ((ctx->pending_outfile || ctx->pending_infile) && !ctx->current_cmd)
	{
		new_cmd = list_new(NULL);
		if (!new_cmd)
			return (NULL);
		assign_pending_redirs(new_cmd, ctx);
		if (!ctx->cmd_list)
			ctx->cmd_list = new_cmd;
		else
			add_bottom(&ctx->cmd_list, new_cmd);
		reinit_pending(ctx);
	}
	return (ctx->cmd_list);
}

// t_com_list	*finalize_pending_redirs(t_parser_context *ctx)
// {
// 	t_com_list	*new_cmd;

// 	if ((ctx->pending_outfile || ctx->pending_infile) && !ctx->current_cmd)
// 	{
// 		new_cmd = list_new(NULL);
// 		if (!new_cmd)
// 			return (NULL);
// 		new_cmd->heredoc_fd = -1;
// 		if (ctx->pending_outfile)
// 		{
// 			new_cmd->outfile = ctx->pending_outfile;
// 			new_cmd->flag_out = ctx->pending_flag_out;
// 			new_cmd->all_outfilles = ctx->pending_all_outfiles;
// 		}
// 		if (ctx->pending_infile)
// 		{
// 			new_cmd->infile = ctx->pending_infile;
// 			new_cmd->flag_in = ctx->pending_flag_in;
// 		}
// 		if (ctx->pending_errfile)
// 			new_cmd->errfile = ctx->pending_errfile;
// 		if (!ctx->cmd_list)
// 			ctx->cmd_list = new_cmd;
// 		else
// 			add_bottom(&ctx->cmd_list, new_cmd);
// 		reinit_pending(ctx);
// 	}
// 	return (ctx->cmd_list);
// }
