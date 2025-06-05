/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_arg.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 13:45:22 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/05 15:36:16 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_arg_token(t_parser_context *ctx)
{
	char	*expanded;
	int		i;

	if (!ctx->current_token || !ctx->current_cmd || !ctx->current_cmd->args)
		return (-1);
	expanded = replace_all_variables(ctx->current_token->value, ctx->envcp, 0);
	if (!expanded)
		return (-1);
	i = 0;
	while (ctx->current_cmd->args[i])
		i++;
	ctx->current_cmd->args[i] = ft_strdup(expanded);
	ctx->current_cmd->args[i + 1] = NULL;
	free(expanded);
	ctx->current_token = ctx->current_token->next;
	return (0);
}
