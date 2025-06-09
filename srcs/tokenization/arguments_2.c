/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arguments_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oceanepique <oceanepique@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 16:13:44 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/09 17:54:34 by oceanepique      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	ensure_newline_at_end(char **str)
{
	char	*tmp;

	if ((*str)[ft_strlen(*str) - 1] != '\n')
	{
		tmp = ft_strjoin(*str, "\n");
		*str = tmp;
	}
	return (0);
}

static int	handle_quote_input(char **str, int stdin_copy)
{
	char	*input;
	int		status;

	g_exit_status = 0;
	signal(SIGINT, handler_sigint_prompt);
	input = readline("> ");
	dup2(stdin_copy, STDIN_FILENO);
	signal(SIGINT, handler_sigint);
	if (g_exit_status == 130)
		return (free(input), 1);
	if (!input)
	{
		ft_putstr_fd("minishell: unexpected EOF while looking for \
matching `''\n", STDERR_FILENO);
		ft_putstr_fd("syntax error: unexpected end of file\n", STDERR_FILENO);
		return (1);
	}
	status = update_str_with_input(str, input);
	free(input);
	if (status == -1)
		return (-1);
	return (0);
}

int	prompt_for_quotes(char **str)
{
	int	status;
	int	stdin_copy;

	stdin_copy = dup(STDIN_FILENO);
	if (stdin_copy == -1)
		return (perror("dup"), -1);
	ensure_newline_at_end(str);
	while (check_mismatched_quotes(*str) == 1)
	{
		status = handle_quote_input(str, stdin_copy);
		if (status != 0)
		{
			close(stdin_copy);
			return (status);
		}
	}
	close(stdin_copy);
	return (g_exit_status);
}
/*
int	prompt_for_quotes(char **str)
{
	int		status;
	int		stdin_copy;
	char	*input;

	stdin_copy = dup(STDIN_FILENO);
	if (stdin_copy == -1)
	{
		perror("dup");
		return (-1);
	}
	ensure_newline_at_end(str);
	while (check_mismatched_quotes(*str) == 1)
	{
		g_exit_status = 0;
		signal(SIGINT, handler_sigint_prompt);
		input = readline("> ");
		dup2(stdin_copy, STDIN_FILENO);
		signal(SIGINT, handler_sigint);
		if (g_exit_status == 130)
		{
			close(stdin_copy);
			return (free(input), 1);
		}
		if (!input)
		{
			ft_putstr_fd("minishell: unexpected EOF while looking for \
				matching `''\n", STDERR_FILENO);
			ft_putstr_fd("syntax error: unexpected end of \
				file\n", STDERR_FILENO);
			close(stdin_copy);
			return (1);
		}
		status = update_str_with_input(str, input);
		free(input);
		if (status == -1)
			return (close(stdin_copy), -1);
	}
	close(stdin_copy);
	return (g_exit_status);
}*/

// Vérification des guillemets
int	check_mismatched_quotes(char *str)
{
	int	single_quote;
	int	double_quote;
	int	i;

	single_quote = 0;
	double_quote = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !double_quote)
			single_quote = !single_quote;
		else if (str[i] == '\"' && !single_quote)
			double_quote = !double_quote;
		i++;
	}
	if (single_quote || double_quote)
		return (1);
	return (0);
}

int	update_str_with_input(char **str, char *input)
{
	char	*tmp;
	char	*join;

	tmp = ft_strjoin(*str, input);
	if (!tmp)
		return (-1);
	if (check_mismatched_quotes(tmp) == 1)
	{
		join = ft_strjoin(tmp, "\n");
		free(tmp);
		if (!join)
			return (-1);
	}
	else
		join = tmp;
	free(*str);
	*str = join;
	return (0);
}
