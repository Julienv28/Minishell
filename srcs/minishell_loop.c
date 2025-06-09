/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_loop.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oceanepique <oceanepique@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:42:18 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/09 17:53:09 by oceanepique      ###   ########.fr       */
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
		if (g_exit_status == 130)
			return (1);
		return (0);
	}
	execute_commands(commands, envcp);
	free_cmd(commands);
	return (1);
}

void	handle_input_error(char *input)
{
	free(input);
	g_exit_status = 0;
}

int	process_input(char *input, char ***envcp)
{
	int	ret;
    
	ret = handle_line(input, envcp);
	if (ret == 1 && g_exit_status == 130)
	{
		handle_input_error(input);
		return (1);
	}
	if (!ret)
	{
		free(input);
		return (0);
	}
	free(input);
	return (1);
}

void	minishell_loop(char ***envcp)
{
	char	*input;
	int		ret;

	while (1)
	{
		set_signal_action();
		input = readline(GREEN "minishell$ " RESET);
		if (!input)
		{
		if (g_exit_status == 130)
		{
			handle_input_error(input);
			continue ;
		}
		exit_shell(*envcp);
	}
	add_history(input);
	ret = process_input(input, envcp);
	if (ret == 1)
		continue;
	}
}
/*
void	minishell_loop(char ***envcp)
{
	char		*input;
    int         ret;

	while (1)
	{
		set_signal_action();
		input = readline(GREEN "minishell$ " RESET);
		if (!input)
		{
			if (g_exit_status == 130)
			{
				free(input);
				g_exit_status = 0;
				continue;
			}
			exit_shell(*envcp);
		}
		add_history(input);
		ret = handle_line(input, envcp);
		if (ret == 1 && g_exit_status == 130)
		{
			// printf("[LOOP] heredoc interrompu, retour au prompt\n");
			g_exit_status = 0;
			continue ;
		}
		if (!ret)
		{
			// printf("[LOOP 5] handle_line a retourné false\n");
			free(input);
			continue ;
		}
		free(input);
	}
}*/
