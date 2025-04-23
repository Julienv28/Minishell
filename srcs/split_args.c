/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oceanepique <oceanepique@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:06:45 by juvitry           #+#    #+#             */
/*   Updated: 2025/04/23 11:39:18 by oceanepique      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// static int is_quote(char c)
// {
//     return (c == '"' || c == '\'');
// }

static int count_words(const char *s, char sep)
{
    int i = 0;
    int count = 0;
    int single_quote = 0;
    int double_quote = 0;

    while (s[i])
    {
        while (s[i] == sep)
            i++;
        if (!s[i])
            break;
        count++;
        while (s[i])
        {
            if (s[i] == '\'' && !double_quote)
                single_quote = !single_quote;
            else if (s[i] == '"' && !single_quote)
                double_quote = !double_quote;
            else if (s[i] == sep && !single_quote && !double_quote)
                break;
            i++;
        }
    }
    return (count);
}

static char *word_dup(const char *s, int start, int end)
{
    char *res = malloc(end - start + 1);
    int i = 0;
    if (!res)
        return (NULL);
    while (start < end)
        res[i++] = s[start++];
    res[i] = '\0';
    return (res);
}

char **split_args(const char *s, char sep)
{
    int i = 0, j = 0, start;
    // int in_quote = 0;
    int single_quote = 0;
    int double_quote = 0;
    char **tab;
    char *raw_word;

    tab = malloc(sizeof(char *) * (count_words(s, sep) + 1));
    if (!tab)
        return (NULL);

    while (s[i])
    {
        while (s[i] == sep)
            i++;
        if (!s[i])
            break;
        start = i;
        while (s[i])
        {
            if (s[i] == '\'' && !double_quote)
                single_quote = !single_quote;
            else if (s[i] == '"' && !single_quote)
                double_quote = !double_quote;
            else if (s[i] == sep && !single_quote && !double_quote)
                break;
            i++;
        }
        raw_word = word_dup(s, start, i);
        tab[j++] = raw_word;
        // free(raw_word);
    }
    tab[j] = NULL;
    return (tab);
}

char *remove_quotes_or_slash(char *str)
{
    int i = 0;
    int j = 0;
    int single_quote;
    int double_quote;
    char *new_str;

    single_quote = 0;
    double_quote = 0;
    new_str = malloc(sizeof(char) * (ft_strlen(str) + 1));
    if (!new_str)
        return (NULL);
    while (str[i])
    {
        if (str[i] == '\'' && !double_quote)
        {
            single_quote = !single_quote;
            i++; // on ne copie pas les quotes simples délimiteuses
        }
        else if (str[i] == '"' && !single_quote)
        {
            double_quote = !double_quote;
            i++; // on ne copie pas les quotes doubles délimiteuses
        }
        else if (str[i] == '\\' && str[i + 1]) // \ suivi d'un caractère
        {
            // gestion de l'antislash sauf dans quotes simples
            new_str[j++] = str[i + 1];
            i += 2;
        }
        else
            new_str[j++] = str[i++];
    }
    new_str[j] = '\0';
    return (new_str);
}
