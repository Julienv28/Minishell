/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oceanepique <oceanepique@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 13:45:38 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/11 16:58:42 by oceanepique      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	init_cmd(t_com *new_cmd, char *expanded)
{
	new_cmd->args = ft_calloc(MAX_ARGS, sizeof(char *));
	if (!new_cmd->args)
		return ;
	new_cmd->args[0] = ft_strdup(expanded);
	new_cmd->args[1] = NULL;
	new_cmd->heredoc_fd = -1;
}

int	is_empty_quoted(char *str)
{
	return (str && ((strcmp(str, "\"\"") == 0) || (strcmp(str, "''") == 0)));
}

static void	assign_redirs(t_parser_context *ctx, t_com *new_cmd)
{
	if (ctx->pending_outfile)
	{
		new_cmd->outfile = ctx->pending_outfile;
		new_cmd->flag_out = ctx->pending_flag_out;
		add_outfile(&new_cmd->all_outfilles, ctx->pending_outfile,
			ctx->pending_flag_out);
		ctx->pending_outfile = NULL;
		ctx->pending_flag_out = -1;
	}
	if (ctx->pending_infile)
	{
		new_cmd->infile = ctx->pending_infile;
		new_cmd->flag_in = ctx->pending_flag_in;
		ctx->pending_infile = NULL;
		ctx->pending_flag_in = -1;
	}
}

static int	create_and_assign_cmd(t_parser_context *ctx, char *expanded)
{
	t_com	*new_cmd;

	new_cmd = list_new(expanded);
	if (!new_cmd)
		return (free(expanded), -1);
	init_cmd(new_cmd, expanded);
	free(expanded);
	assign_redirs(ctx, new_cmd);
	if (!ctx->cmd_list)
		ctx->cmd_list = new_cmd;
	else
		add_bottom(&ctx->cmd_list, new_cmd);
	ctx->current_cmd = new_cmd;
	return (0);
}

int	handle_cmd_token(t_parser_context *ctx)
{
	char	*expanded;

	if (!ctx->current_token->value)
	{
		ctx->current_token = ctx->current_token->next;
		return (0);
	}
	expanded = replace_var(ctx->current_token->value,
			ctx->envcp, 0, 1);
	if (!expanded)
		return (-1);
	if (expanded[0] == '\0')
	{
		if (is_empty_quoted(ctx->current_token->value))
			g_exit_status = 127;
		free(expanded);
		ctx->current_token = ctx->current_token->next;
		return (0);
	}
	if (create_and_assign_cmd(ctx, expanded) == -1)
		return (-1);
	ctx->current_token = ctx->current_token->next;
	return (0);
}

/*
int	handle_cmd_token(t_parser_context *ctx)
{
	char		*expanded;
	t_com	*new_cmd;

	if (!ctx->current_token->value)
	{
		ctx->current_token = ctx->current_token->next;
		return (0);
	}
	expanded = replace_var(ctx->current_token->value,
			ctx->envcp, 0, 1);
	if (!expanded)
		return (-1);
	if (expanded[0] == '\0')
	{
		if (is_empty_quoted(ctx->current_token->value))
			g_exit_status = 127;
		free(expanded);
		ctx->current_token = ctx->current_token->next;
		return (0);
	}
	new_cmd = list_new(expanded);
	if (!new_cmd)
		return (free(expanded), -1);
	init_cmd(new_cmd, expanded);
	free(expanded);
	assign_redirs(ctx, new_cmd);
	if (!ctx->cmd_list)
		ctx->cmd_list = new_cmd;
	else
		add_bottom(&ctx->cmd_list, new_cmd);
	ctx->current_cmd = new_cmd;
	ctx->current_token = ctx->current_token->next;
	return (0);
}*/
