/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_loop.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:42:18 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/06 15:48:50 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	parse_input(char *input, char **envp, t_token **tokens)
{
	*tokens = create_tokens(&input, envp);
	if (!*tokens)
		return (0);
	if (!handle_null_tokens(*tokens, input))
	{
		free_tokens(*tokens);
		return (0);
	}
	return (1);
}

static int	handle_line(char *input, char ***envcp)
{
	t_token		*tokens;
	t_com_list	*commands;

	if (!parse_input(input, *envcp, &tokens))
		return (0);
	commands = tokens_to_cmds(tokens, *envcp);
	free_tokens(tokens);
	if (!commands)
		return (0);
	execute_commands(commands, envcp);
	free_cmd(commands);
	return (1);
}

void	minishell_loop(char ***envcp)
{
	char		*input;

	while (1)
	{
		set_signal_action();
		input = readline(GREEN "minishell$ " RESET);
		if (!input)
			exit_shell(*envcp);
		add_history(input);
		if (!handle_line(input, envcp))
		{
			free(input);
			continue ;
		}
		free(input);
	}
}
