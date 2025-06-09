/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arguments.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opique <opique@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:06:51 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/09 11:12:11 by opique           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ensure_newline_at_end(char *str)
{
	char	*tmp;

	if ((*str)[ft_strlen(*str) - 1] != '\n')
	{
		tmp = ft_strjoin(*str, "\n");
		//free(*str);
		*str = tmp;
	}
	return (0);
}

char	*prompt_for_quotes(char *str)
{
	char	*input2;
	char	*tmp;
	int		status;
	int		stdin_copy;

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
		status = update_str_with_input(&str, input2);
		free(input2);
		if (status == -1)
			return (close(stdin_copy), -1);
	}
	close(stdin_copy);
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
	int	status;

	if (check_mismatched_quotes(*str) == 1)
	{
		status = prompt_for_quotes(str);
		if (status == -1)
		{
			g_exit_status = 1;
			return (-1);
		}
		else if (status == 1)
			return (1);
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
	int		quote_status;

	start = *i;
	quote_status = handle_quotes(str);
	if (quote_status == -1)
		return (-1);
	if (quote_status == 1)
		return (1);
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
