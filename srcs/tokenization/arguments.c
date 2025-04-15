/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arguments.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opique <opique@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:06:51 by juvitry           #+#    #+#             */
/*   Updated: 2025/04/15 10:55:04 by opique           ###   ########.fr       */
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

// Fonction pour gérer les guillemets ouvrir un prompt <
int prompt_for_quotes(char **str)
{
    char *input;
    char *tmp;

    tmp = NULL;
    while (check_mismatched_quotes(*str) == 1)
    {
        input = readline("> ");
        if (g_exit_status == 130 || !input || input[0] == '\0')
        {
            g_exit_status = 0;
            free(input);
            return (-1);
        }
        tmp = ft_strjoin(*str, input);
        free(*str);
        *str = ft_strjoin(tmp, "\n");
        free(tmp);
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
