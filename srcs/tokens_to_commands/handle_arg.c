/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_arg.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 13:45:22 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/16 11:37:47 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_arg_token(t_parser_context *ctx, t_msh *msh)
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
		expanded = replace_var(ctx->current_token->value,
				msh->envcp, 0, 1);
	}
	if (!expanded)
		return (-1);
	expanded = remove_quotes_or_slash(expanded);
	i = 0;
	while (ctx->current_cmd->args[i])
		i++;
	ctx->current_cmd->args[i] = ft_strdup(expanded);
	ctx->current_cmd->args[i + 1] = NULL;
	free(expanded);
	ctx->current_token = ctx->current_token->next;
	return (0);
}
