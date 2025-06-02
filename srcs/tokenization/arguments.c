/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arguments.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opique <opique@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:06:51 by juvitry           #+#    #+#             */
/*   Updated: 2025/06/02 16:19:16 by opique           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

int prompt_for_quotes(char **str)
{
    char *input;
    char *tmp;
    char *join;

    ensure_newline_at_end(str);
    while (check_mismatched_quotes(*str) == 1)
    {
		signal(SIGINT, heredoc_sigint_handler); // handler temporaire
        input = readline("> ");
		signal(SIGINT, handler_sigint);
		if (g_exit_status == 130)
        {
            free(input);  // readline retourne une ligne vide, qu'on ignore
            return -1;    // on quitte prompt_for_quotes proprement
        }
		if (!input)
		{
			ft_putstr_fd("minishell: unexpected EOF while looking for matching `''\n", STDERR_FILENO);
            ft_putstr_fd("syntax error: unexpected end of file\n", STDERR_FILENO);
            return (-1);
		}
        tmp = ft_strjoin(*str, input);
        if (check_mismatched_quotes(tmp) == 1)
        {
            join = ft_strjoin(tmp, "\n");
            free(tmp);
        }
        else
            join = tmp;
        free(*str);
        *str = join;
        free(input);
    }
    return (g_exit_status);
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

int	extract_word(char **str, int *i, char **word, int *start)
{
	while ((*str)[*i] && (*str)[*i] != ' ' && (*str)[*i] != '|' &&
			(*str)[*i] != '<' && (*str)[*i] != '>')
	{
		if ((*str)[*i] == '\'')
		{
			(*i)++;
			while ((*str)[*i] && (*str)[*i] != '\'')
				(*i)++;
			(*i)++;
		}
		else if ((*str)[*i] == '\"')
		{
			(*i)++;
			while ((*str)[*i] && (*str)[*i] != '\"')
				(*i)++;
			(*i)++;
		}
		else
			(*i)++;
	}
	*word = ft_strndup(*str + *start, *i - *start);
	return (0);
}

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
}
