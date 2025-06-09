/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arguments.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oceanepique <oceanepique@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:06:51 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/09 16:14:36 by oceanepique      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	concat_arg_following(char **str, int *i, t_token *token)
{
	int		extra_start;
	char	*extra_word;
	char	*joined;

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

static int	handle_quotes(char **str)
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

// VERSION QUI MACHE AVEC HEREDOC
int	extract_word(char **str, int *i, char **word, int *start)
{
	int		j;
	int		has_quotes;
	int		tab[2];

	has_quotes = 0;
	tab[0] = 0;
	tab[1] = 0;
	j = *i;
	while ((*str)[j])
	{
		if ((*str)[j] == '\'' && !tab[1])
			tab[0] = !tab[0];
		else if ((*str)[j] == '"' && !tab[0])
			tab[1] = !tab[1];
		else if (!tab[0] && !tab[1] &&
			(ft_isspace((*str)[j]) || (*str)[j] == '|' ||
			(*str)[j] == '<' || (*str)[j] == '>'))
			break ;
		j++;
	}
	*word = ft_strndup(*str + *start, j - *start);
	if (!*word)
		return (-1);
	*i = j;
	return (has_quotes || tab[0] || tab[1]);
}

static int	init_and_extract_word(char **str, int *i, char **word, int *is_quoted)
{
	int	start;
	int	quote_status;

	start = *i;
	quote_status = handle_quotes(str);
	if (quote_status != 0)
		return (quote_status);
	*is_quoted = extract_word(str, i, word, &start);
	if (!*word)
		return (-1);
	return (0);
}

int	handle_word(char **str, int *i, t_token **tokens, int *expect_cmd)
{
	int		type;
	char	*word;
	int		is_quoted;
	t_token	*new;
	int		status;

	status = init_and_extract_word(str, i, &word, &is_quoted);
	if (status != 0)
		return (status);
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
}*/
