/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oceanepique <oceanepique@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:42:01 by juvitry           #+#    #+#             */
/*   Updated: 2025/05/16 14:32:55 by oceanepique      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void ft_echo(char **args)
{
    int i = 1;
    int newline = 1;

    // Gère tous les -n consécutifs
    while (args[i] && is_valid_n_flag(args[i]))
    {
        newline = 0;
        i++;
    }

    while (args[i])
    {
        ft_putstr_fd(args[i], STDOUT_FILENO);
        if (args[i + 1])
            ft_putchar_fd(' ', STDOUT_FILENO);
        i++;
    }

    if (newline)
        ft_putchar_fd('\n', STDOUT_FILENO);
}

char *get_value_cleaned(char *name, char **envp)
{
    char *raw;

    // printf("Recherche de : %s\n", name);
    raw = get_env_value(name, envp);
    // printf("Valeur trouvée : %s\n", raw);
    if (!raw)
        return (NULL);
    return (clean_spaces(raw));
}

char *get_env_value(char *name, char **envp)
{
    int i;
    size_t len;

    i = 0;
    len = ft_strlen(name);
    if (!name || !envp)
        return (NULL);
    while (envp[i])
    {
        if (envp[i] && ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
            return (envp[i] + len + 1);
        i++;
    }
    return (NULL);
}

int parse_args_echo(char **args)
{
    int i;

    if (count_ags(args) == 1)
    {
        ft_putchar_fd('\n', 1);
        return (1);
    }
    else if (ft_strncmp(args[1], "-n", 2) == 0 && ft_strlen(args[1]) != 2 && count_ags(args) == 2)
    {
        i = 2;
        while (args[1][i])
        {
            if (args[1][i] != 'n')
                return (0);
            i++;
        }
        return (1);
    }
    else if (ft_strcmp(args[1], "-n") == 0 && count_ags(args) > 2)
    {
        i = 2;
        while (args[i])
        {
            if (ft_strcmp(args[i], "-n") != 0)
                return (0);
            i++;
        }
        return (1);
    }
    else
        return (0);
}

// Trimmer les variables et nettoyer les espaces inutiles
char *clean_spaces(char *str)
{
    int i = 0, j = 0;
    int in_space = 0;
    char *trimmed;
    char *res;

    while (str[i] && ft_isspace(str[i]))
        i++;
    trimmed = ft_strdup(str + i);
    if (!trimmed)
        return (NULL);

    res = malloc(ft_strlen(trimmed) + 1); // +1 pour le \0
    if (!res)
    {
        free(trimmed);
        return (NULL);
    }

    i = 0;
    while (trimmed[i])
    {
        if (ft_isspace(trimmed[i]))
        {
            if (!in_space)
                res[j++] = ' ';
            in_space = 1;
        }
        else
        {
            res[j++] = trimmed[i];
            in_space = 0;
        }
        i++;
    }

    if (j > 0 && res[j - 1] == ' ')
        j--;
    res[j] = '\0';
    free(trimmed);
    return (res);
}

/*
static int	ft_isupper(int c)
{
    if (c >= 65 && c <= 90)
        return (1);
    else
        return (0);
}

void ft_echo(char *str, char **envp)
{
    int i = 0;

    if (!str || str[0] == '\0')
    {
        ft_putchar_fd('\n', 1);
        return ;
    }
    if (str[i] == '\'' || str[i] == '\"')
        i++;
    while (str[i])
    {
        if ((str[i] == '$' && str[0] != '\"') || (str[i] == '$' && str[0] == '\''))
        {
            i++;
            int start = i;
            while (str[i] && (ft_isupper(str[i]) || str[i] == '_'))
                i++;
            int len = i - start;
            char *var = malloc(len + 1);
            if (!var)
                return ;
            for (int j = 0; j < len; j++)
                var[j] = str[start + j];
            var[len] = '\0';

            char *val = get_env_value(var, envp);
            if (val)
                ft_putstr_fd(val, 1);
            free(var);
        }
        else if ((str[i] == '\'' && str[0] != '\"') || (str[0] == '\"'
            && ((str[i] == '\'' || str[i] == '\"') && str[i + 1] == '\0')))
            i++;
        else
        {
            ft_putchar_fd(str[i], 1);
            i++;
        }
    }
}*/