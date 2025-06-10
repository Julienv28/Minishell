/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_arg.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opique <opique@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 13:45:22 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/10 12:29:42 by opique           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_arg_token(t_parser_context *ctx)
{
	char	*expanded;
	int		i;

	if (!ctx->current_token || !ctx->current_cmd || !ctx->current_cmd->args)
		return (-1);
	if (ctx->current_cmd->args[0]
		&& ft_strcmp(ctx->current_cmd->args[0], "export") == 0)
		expanded = ft_strdup(ctx->current_token->value);
	else
	{
		expanded = replace_all_variables(ctx->current_token->value,
				ctx->envcp, 0, 1);
	}
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
