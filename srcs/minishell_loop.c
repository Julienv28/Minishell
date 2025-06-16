/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_loop.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oceanepique <oceanepique@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:42:18 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/16 19:46:52 by oceanepique      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	parse_input(char *input, t_msh *msh)
{
	msh->tkn = create_tokens(&input, msh);
	if (!msh->tkn)
		return (0);
	if (!handle_null_tokens(msh, input))
	{
		free_tokens(msh->tkn);
		return (0);
	}
	return (1);
}

static int	handle_line(char *input, t_msh *msh)
{
	if (!parse_input(input, msh))
		return (0);
	msh->com = tokens_to_cmds(msh);
	free_tokens(msh->tkn);
	if (!msh->com)
	{
		if (g_sig_status == 130)
			return (1);
		return (0);
	}
	execute_commands(msh);
	free_cmd(msh->com);
	return (1);
}

static int	process_input(char *input, t_msh *msh)
{
	int	ret;

	ret = handle_line(input, msh);
	if (ret == 1 && msh->ex_status == 130)
		return (1);
	if (!ret)
	{
		free(input);
		return (0);
	}
	free(input);
	return (1);
}

void	minishell_loop(t_msh *msh)
{
	char	*input;
	int		ret;

	while (1)
	{
		input = get_input(msh);
		if (!input)
			exit_shell(msh);
        if (input[0] == '\0' && msh->ex_status == 130)
		{
			free(input);
			continue ;
		}
		add_history(input);
		ret = process_input(input, msh);
		if (ret == 1)
			continue ;
	}
}

