/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 13:46:30 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/09 17:22:30 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
/*
static int	handle_heredoc_redir(t_parser_context *ctx, char *filename)
{
	char	*expanded;
	char	*heredoc_name;
	int		expand;

	expand = !limiter_is_quoted(filename);
	expanded = replace_all_variables(filename, ctx->envcp, 0);
	if (!expanded)
		return (-1);
	heredoc_name = handle_heredoc(expanded, ctx->envcp, expand);
	free(expanded);
	free(filename);
	if (!heredoc_name)
		return (-1);
	if (!ctx->current_cmd)
		ctx->pending_infile = ft_strdup(heredoc_name);
	else
	{
		free(ctx->current_cmd->infile);
		ctx->current_cmd->infile = ft_strdup(heredoc_name);
	}
	free(heredoc_name);
	return (0);
}*/

// A Deplacer dans un autre fichier (voire en creer un juste pour son decoupage)
static int	handle_heredoc_redir(t_parser_context *ctx, char *filename)
{
	char	*heredoc_name;
	char 	*cleaned_limiter;
	int		expand_var = 0;

	printf("[HEREDOC_REDIR 1] Début handle_heredoc_redir, filename='%s'\n", filename);
	expand_var = !limiter_is_quoted(filename);
	printf("[HEREDOC_REDIR 2] expand_var=%d\n", expand_var);
	cleaned_limiter = remove_quotes_or_slash(filename);
    if (!cleaned_limiter)
	{
		//printf("[HEREDOC_REDIR 3] cleaned_limiter NULL, free filename et return -1\n");
		return (free(filename), -1);
	}
	free(filename);
	printf("[HEREDOC_REDIR 4] cleaned_limiter='%s'\n", cleaned_limiter);
	heredoc_name = handle_heredoc(cleaned_limiter, ctx->envcp, expand_var);
	free(cleaned_limiter);
	if (!heredoc_name)
	{
		if (g_exit_status == 130)
		{
			// interruption heredoc, pas une erreur fatale : juste retour spécial
			printf("[HEREDOC_REDIR] heredoc interrompu par Ctrl+C\n");
			return (1);  // retour spécial pour interruption Ctrl+C
		}
		printf("[HEREDOC_REDIR 5] handle_heredoc a échoué (NULL), return -1\n");
		return (-1);
	}
	//printf("[HEREDOC_REDIR 6] heredoc_name='%s'\n", heredoc_name);
	if (!ctx->current_cmd)
	{
		ctx->pending_infile = ft_strdup(heredoc_name);
		printf("[HEREDOC_REDIR 7] pending_infile set to '%s'\n", ctx->pending_infile);
	}
	else
	{
		free(ctx->current_cmd->infile);
		ctx->current_cmd->infile = ft_strdup(heredoc_name);
		printf("[HEREDOC_REDIR 8] current_cmd->infile set to '%s'\n", ctx->current_cmd->infile);
	}
	free(heredoc_name);
	printf("[HEREDOC_REDIR 9] Fin handle_heredoc_redir\n");
	return (0);
}

static int	handle_input_redir(t_parser_context *ctx, char *filename)
{
	char	*expanded;

	expanded = replace_all_variables(filename, ctx->envcp, 0, 1);
	free(filename);
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
		free(filename);
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
	return (free(filename), 0);
}

static int	get_redir_filename(t_parser_context *ctx, char **filename)
{
	if (!ctx->current_token || ctx->current_token->type != ARG)
	{
		syntax_error();
		return (-1);
	}
	*filename = ft_strdup(ctx->current_token->value);
	if (!*filename)
		return (-1);
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

	redir_type = ctx->current_token->type;
	ctx->current_token = ctx->current_token->next;
	if (get_redir_filename(ctx, &filename) < 0)
		return (-1);
	ret = exec_redir(ctx, redir_type, filename);
	if (ret != 0)
		return (ret);
	ctx->current_token = ctx->current_token->next;
	return (0);
}

// int	handle_redir_token(t_parser_context *ctx)
// {
// 	int		redir_type = ctx->current_token->type;
// 	char	*filename;
// 	char	*expanded;

// 	ctx->current_token = ctx->current_token->next;
// 	if (!ctx->current_token || ctx->current_token->type != ARG)
// 		return (syntax_error(), -1);

// 	filename = ft_strdup(ctx->current_token->value);
// 	if (!filename)
// 		return (-1);

// 	if (redir_type == HEREDOC)
// 	{
// 		int expand = !limiter_is_quoted(filename);
// 		expanded = replace_all_variables(filename, ctx->envcp, 0);
// 		char *heredoc_name = handle_heredoc(expanded, ctx->envcp, expand);
// 		free(expanded);
// 		free(filename);
// 		if (!heredoc_name)
// 			return (-1);
// 		if (!ctx->current_cmd)
// 			ctx->pending_infile = ft_strdup(heredoc_name);
// 		else
// 		{
// 			free(ctx->current_cmd->infile);
// 			ctx->current_cmd->infile = ft_strdup(heredoc_name);
// 		}
// 		free(heredoc_name);
// 	}
// 	else if (redir_type == INPUT)
// 	{
// 		expanded = replace_all_variables(filename, ctx->envcp, 0);
// 		free(filename);
// 		if (ctx->current_cmd)
// 		{
// 			free(ctx->current_cmd->infile);
// 			ctx->current_cmd->infile = ft_strdup(expanded);
// 		}
// 		else
// 		{
// 			free(ctx->pending_infile);
// 			ctx->pending_infile = ft_strdup(expanded);
// 		}
// 		free(expanded);
// 	}
// 	else	free(ctx->current_cmd->outfile);
// 			ctx->current_cmd->outfile = ft_strdup(filename);
// 			ctx->current_cmd->flag_out = flag;
// 		}
// 		else
// 		{
// 			free(ctx->pending_outfile);
// 			ctx->pending_outfile = ft_strdup(filename);
// 			ctx->pending_flag_out = flag;
// 		}
// 		free(filename);
// 	}
// 	ctx->current_token = ctx->current_token->next;
// 	return (0);
// }
// 			perror(filename);
// 			free(filename);
// 			return (-1);
// 		}
// 		close(fd);
// 		if (ctx->current_cmd)
// 		{
// 			free(ctx->current_cmd->outfile);
// 			ctx->current_cmd->outfile = ft_strdup(filename);
// 			ctx->current_cmd->flag_out = flag;
// 		}
// 		else
// 		{
// 			free(ctx->pending_outfile);
// 			ctx->pending_outfile = ft_strdup(filename);
// 			ctx->pending_flag_out = flag;
// 		}
// 		free(filename);
// 	}
// 	ctx->current_token = ctx->current_token->next;
// 	return (0);
// }
