/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arguments.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oceanepique <oceanepique@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:06:51 by juvitry           #+#    #+#             */
/*   Updated: 2025/05/21 10:52:46 by oceanepique      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Vérification des guillemets
int check_mismatched_quotes(char *str)
{
    int single_quote = 0;
    int double_quote = 0;
    int i = 0;

    // Vérifier les guillemets mal appariés
    while (str[i])
    {
        if (str[i] == '\'' && !double_quote)
            single_quote = !single_quote;
        else if (str[i] == '\"' && !single_quote)
            double_quote = !double_quote;
        i++;
    }
    // Si les guillemets sont mal fermés, cela veut dire que l'ordre ne correspond pas
    if (single_quote || double_quote)
        return (1); // Mismatched quotes
    return (0);
}

int prompt_for_quotes(char **str)
{
    char *input;
    char *tmp;
    char *join;

    tmp = NULL;
    // S'assurer que la première ligne se termine par \n
    if ((*str)[ft_strlen(*str) - 1] != '\n')
    {
        tmp = ft_strjoin(*str, "\n");
        free(*str);
        *str = tmp;
    }
    while (check_mismatched_quotes(*str) == 1)
    {
        input = readline("> ");
        if (!input)
        {
            ft_putstr_fd("minishell: unexpected EOF while looking for matching `''\n", STDERR_FILENO);
            ft_putstr_fd("syntax error: unexpected end of file\n", STDERR_FILENO);
            free(input);
            return (-1);
        }
        else if (g_exit_status == 130)
        {
            ft_putstr_fd("ctrl c propnt\n", STDERR_FILENO);
            g_exit_status = 0;
            free(input);
            return (-1);
        }

        tmp = ft_strjoin(*str, input);
        if (check_mismatched_quotes(tmp) == 1)
        {
            // pas ferme donc \n
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

// Fonction pour gérer la commande et les arguments
int handle_word(char **str, int *i, t_token **tokens, int *expect_cmd)
{
    int start;
    int type;
    char *word;

    if (check_mismatched_quotes(*str) == 1)
    {
        // printf("Syntax error: mismatched quotes detected.\n");
        if (prompt_for_quotes(str) == -1)
            return (-1);
    }
    start = *i;
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
    word = ft_strndup(*str + start, *i - start);
    if (*expect_cmd)
        type = CMD;
    else
        type = ARG;
    add_token(tokens, word, type);
    free(word);
    *expect_cmd = 0;
    return (0);
}
