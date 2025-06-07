/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arguments.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pique <pique@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:06:51 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/07 14:37:36 by pique            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ensure_newline_at_end(char **str)
{
	char	*tmp;

	if ((*str)[ft_strlen(*str) - 1] != '\n')
	{
		tmp = ft_strjoin(*str, "\n");
		free(*str);
		*str = tmp;
	}
	return (0);
}

int	prompt_for_quotes(char **str)
{
	char	*input;
	int		status;

	ensure_newline_at_end(str);
	while (check_mismatched_quotes(*str) == 1)
	{
		signal(SIGINT, heredoc_sigint_handler);
		input = readline("> ");
		signal(SIGINT, handler_sigint);
		if (g_exit_status == 130)
			return (free(input), -1);
		if (!input)
		{
			ft_putstr_fd("minishell: unexpected EOF while looking for \
				matching `''\n", STDERR_FILENO);
			ft_putstr_fd("syntax error: unexpected end of \
				file\n", STDERR_FILENO);
			return (-1);
		}
		status = update_str_with_input(str, input);
		if (status == -1)
			return (-1);
	}
	return (g_exit_status);
}

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

int	handle_quotes(char **str)
{
	if (check_mismatched_quotes(*str) == 1)
	{
		if (prompt_for_quotes(str) == -1)
			return (-1);
	}
	return (0);
}

// ✅ Patch : si on vient d’ajouter un ARG avec un '=', concatène les ARG suivants
int	concat_arg_following(char **str, int *i, t_token *token)
{
	int extra_start;
	char *extra_word;
	char *joined;

	extra_word = NULL;
	while ((*str)[*i] && !ft_isspace((*str)[*i]) &&
			(*str)[*i] != '|' && (*str)[*i] != '<' && (*str)[*i] != '>')
	{
		extra_start = *i;
		if (extract_word(str, i, &extra_word, &extra_start) == -1)
			return (-1);
		if (!extra_word)
			return (-1);
		joined = ft_strjoin(token->value, extra_word);
		free(token->value);
		free(extra_word);
		token->value = joined;
		if (!token->value)
			return (-1);
	}
	return (0);
}

int	handle_word(char **str, int *i, t_token **tokens, int *expect_cmd)
{
	int		start;
	int		type;
	char	*word;
	int		is_quoted;
	t_token *new;

	start = *i;
	if (handle_quotes(str) == -1)
		return (-1);
	is_quoted = extract_word(str, i, &word, &start);
	if (!word)
		return (-1);
	if (*expect_cmd)
		type = CMD;
	else
		type = ARG;
	*expect_cmd = 0;
	new = add_token(tokens, word, type, is_quoted);
	free(word);
	if (!new)
		return (-1);
	if (type == ARG && ft_strchr(new->value, '='))
		if (concat_arg_following(str, i, new) == -1)
			return (-1);
	return (0);
}


/*
int	handle_word(char **str, int *i, t_token **tokens, int *expect_cmd)
{
	int		start;
	int		type;
	char	*word;

	start = *i;
	if (handle_quotes(str) == -1)
		return (-1);
	extract_word(str, i, &word, &start);
	if (*expect_cmd)
		type = CMD;
	else
		type = ARG;
	add_token(tokens, word, type);
	free(word);
	*expect_cmd = 0;
	return (0);
}*/
