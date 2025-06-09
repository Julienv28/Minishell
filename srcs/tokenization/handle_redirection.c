/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oceanepique <oceanepique@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 09:31:19 by opique            #+#    #+#             */
/*   Updated: 2025/06/09 17:55:42 by oceanepique      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	process_redirection_value(int type, char *word, t_token **tokens, char **envcp)
{
	char	*final;
	int		is_quoted;
	char	*cleaned_limiter;

	final = NULL;
	is_quoted = limiter_is_quoted(word);
	if (type == HEREDOC && is_quoted)
		return (add_token(tokens, word, ARG, 1), 1);
	else if (type == HEREDOC)
	{
		cleaned_limiter = remove_quotes_or_slash(word);
		if (!cleaned_limiter)
			return (free(word), -1);
		if (!add_token(tokens, ft_strdup(cleaned_limiter), ARG, 0))
			return (free(cleaned_limiter), free(word), -1);
		free(cleaned_limiter);
		return (1);
	}
	final = expand_clean_word(word, envcp);
	if (!final)
		return (free(word), -1);
	add_token(tokens, final, ARG, 0);
	free(final);
	free(word);
	return (1);
}

// Convertir un type en symbole de redirection
char	*add_symbol(int type)
{
	if (type == HEREDOC)
		return (ft_strdup("<<"));
	else if (type == APPEND)
		return (ft_strdup(">>"));
	else if (type == INPUT)
		return (ft_strdup("<"));
	else if (type == TRUNC)
		return (ft_strdup(">"));
	else if (type == ERR_REDIR)
		return (ft_strdup("2>"));
	return (NULL);
}

char	*extract_redir_word(char *str, int *i)
{
	int	start;

	while (str[*i] == ' ')
		(*i)++;
	start = *i;
	if (!str[*i] || str[*i] == '<' || str[*i] == '>' || str[*i] == '|')
		return (NULL);
	while (str[*i] && str[*i] != ' ' && str[*i] != '|'
		&& str[*i] != '<' && str[*i] != '>')
		(*i)++;
	if (*i == start)
		return (NULL);
	return (ft_strndup(str + start, *i - start));
}

// Analyser les redirections et avancer l'index
int	parse_redirection(char *str, int *i)
{
	if (str[*i] == '2' && str[*i + 1] == '>' && str[*i + 2] == '>')
		return ((*i) += 3, ERR_REDIR);
	else if (str[*i] == '2' && str[*i + 1] == '>')
		return ((*i) += 2, ERR_REDIR);
	if (str[*i] == '<' && str[*i + 1] == '<')
		return ((*i) += 2, HEREDOC);
	else if (str[*i] == '>' && str[*i + 1] == '>')
		return ((*i) += 2, APPEND);
	else if (str[*i] == '<')
		return ((*i)++, INPUT);
	else if (str[*i] == '>')
		return ((*i)++, TRUNC);
	return (0);
}

int	handle_redirection(char *str, int *i, t_token **tokens, char **envcp)
{
	int		type;
	char	*symbol;
	char	*word;

	symbol = NULL;
	word = NULL;
	type = parse_redirection(str, i);
	if (!type)
		return (0);
	if (check_redirection(str, i) == -1)
		return (g_exit_status = 2, -1);
	word = extract_redir_word(str, i);
	if (!word)
	{
		g_exit_status = 1;
		return (printf("minishell: syntax error near redirection\n"), -1);
	}
	symbol = add_symbol(type);
	if (!symbol)
		return (free(word), -1);
	add_token(tokens, symbol, type, 0);
	free(symbol);
	return (process_redirection_value(type, word, tokens, envcp));
}
