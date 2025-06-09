/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_loop.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opique <opique@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:42:18 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/09 12:14:29 by opique           ###   ########.fr       */
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
	{
		// Ici, on pourrait vérifier g_exit_status pour voir si c'est une interruption heredoc
		if (g_exit_status == 130)
			return (1); // heredoc interrompu
		return (0);
	}
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
		{
			if (g_exit_status == 130) // ← interrompu par SIGINT
			{
				free(input);
				g_exit_status = 0;
				continue;
			}
			printf("[LOOP 2] readline a retourné NULL -> exit_shell\n");
			exit_shell(*envcp);
		}
		printf("[LOOP 3] Input reçu : '%s'\n", input);
		add_history(input);
		int ret = handle_line(input, envcp);
		if (ret == 1 && g_exit_status == 130)
		{
			printf("[LOOP] heredoc interrompu, retour au prompt\n");
			g_exit_status = 0;
			free(input);
			continue ;
		}
		if (!ret)
		{
			printf("[LOOP 5] handle_line a retourné false\n");
			free(input);
			continue ;
		}
		free(input);
	}
}
