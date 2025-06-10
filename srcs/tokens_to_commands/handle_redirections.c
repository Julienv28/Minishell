/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opique <opique@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 13:46:30 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/10 16:38:59 by opique           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	handle_heredoc_redir(t_parser_context *ctx, char *filename)
{
	char	*heredoc_name;
	char	*cleaned_limiter;
	int		expand_var;
	int		ret;

	expand_var = !limiter_is_quoted(filename);
	cleaned_limiter = remove_quotes_or_slash(filename);
	if (!cleaned_limiter)
		return (-1);
	heredoc_name = handle_heredoc(cleaned_limiter, ctx->envcp, expand_var);
	if (!heredoc_name)
	{
		if (g_exit_status == 130)
			return (1);
		return (-1);
	}
	free (cleaned_limiter);
	ret = assign_heredoc_to_ctx(ctx, heredoc_name);
	free(heredoc_name);
	return (ret);
}

static int	handle_input_redir(t_parser_context *ctx, char *filename)
{
	char	*expanded;

	expanded = replace_all_variables(filename, ctx->envcp, 0, 1);
	if (!expanded)
		return (-1);
	if (ctx->current_cmd)
	{
		free(ctx->current_cmd->infile);
		ctx->current_cmd->infile = ft_strdup(expanded);
	}
	else
	{
		free(ctx->pending_infile);
		ctx->pending_infile = ft_strdup(expanded);
	}
	free(expanded);
	return (0);
}

static int	handle_outdir(t_parser_context *ctx, char *filename, int redir_type)
{
	int	flag;
	int	fd;

	flag = (redir_type == APPEND);
	fd = open_outfile(filename, flag);
	if (fd < 0)
	{
		perror(filename);
		return (-1);
	}
	close(fd);
	if (ctx->current_cmd)
	{
		free(ctx->current_cmd->outfile);
		ctx->current_cmd->outfile = ft_strdup(filename);
		ctx->current_cmd->flag_out = flag;
	}
	else
	{
		free(ctx->pending_outfile);
		ctx->pending_outfile = ft_strdup(filename);
		ctx->pending_flag_out = flag;
	}
	return (0);
}

static int	exec_redir(t_parser_context *ctx, int type, char *filename)
{
	int	ret;

	if (type == HEREDOC)
	{
		ret = handle_heredoc_redir(ctx, filename);
		if (ret == 1 || ret < 0)
			return (ret);
	}
	else if (type == INPUT)
		ret = handle_input_redir(ctx, filename);
	else
		ret = handle_outdir(ctx, filename, type);
	if (ret < 0)
		return (-1);
	return (0);
}

int	handle_redir_token(t_parser_context *ctx)
{
	int		redir_type;
	char	*filename;
	int		ret;

	ret = 0;
	redir_type = ctx->current_token->type;
	ctx->current_token = ctx->current_token->next;
	if (!ctx->current_token || ctx->current_token->type != ARG)
		return (syntax_error(), -1);
	filename = ft_strdup(ctx->current_token->value);
	if (!filename)
		return (-1);
	ret = exec_redir(ctx, redir_type, filename);
	free(filename);
	if (ret != 0)
		return (ret);
	ctx->current_token = ctx->current_token->next;
	return (0);
}
