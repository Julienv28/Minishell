/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 13:46:14 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/05 15:12:11 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_pipe_token(t_parser_context *ctx)
{
	if (ctx->current_cmd)
		ctx->current_cmd->is_pipe = 1;
	ctx->current_cmd = NULL;
	ctx->current_token = ctx->current_token->next;
}
