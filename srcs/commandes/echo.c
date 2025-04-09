/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opique <opique@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:42:01 by juvitry           #+#    #+#             */
/*   Updated: 2025/04/09 14:03:13 by opique           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int ft_isupper(int c)
{
    if (c >= 65 && c <= 90)
        return (1);
    else
        return (0);
}

// void ft_echo(char *str, char **envp)
// {
//     int i;
//     int j;
//     int k;
//     char *env_str;

//     env_str = NULL;
//     i = 0;
//     while (str[i])
//     {
//         if (str[0] == '\"' && str[i] == '$' && ft_isupper(str[i + 1]) == 1)
//         {
//             j = 0;
//             while (str[i] != ' ')
//             {
//                 str[i] = env_str[j];
//                 i++;
//                 j++;
//             }
//             j = 0;
//             while (envp[j])
//             {
//                 if (ft_strncmp(env_str, envp[j], ft_strlen(env_str)) == 0)
//                 {
//                     k = ft_strlen(env_str);
//                     if (envp[j][k] == 61)
//                         k++;
//                     while (envp[j][k])
//                     {
//                         ft_putchar_fd(envp[j][k], 1);
//                         k++;
//                     }
//                     break;
//                 }
//                 j++;
//             }
//             ft_putstr_fd(env_str, 1);
//         }
//         else
//             ft_putchar_fd(str[i], 1);
//         i++;
//     }
//}

char *get_env_value(char *name, char **envp)
{
    int i = 0;
    size_t len = ft_strlen(name);

    while (envp[i])
    {
        if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
            return (envp[i] + len + 1);
        i++;
    }
    return (NULL);
}

void ft_echo(char *str, char **envp)
{
    int i = 0;

    while (str[i])
    {
        if (str[i] == '$' && ft_strchr(str, '\'') == 0)
        {
            i++;
            int start = i;
            while (str[i] && (ft_isupper(str[i]) || str[i] == '_'))
                i++;
            int len = i - start;
            char *var = malloc(len + 1);
            if (!var)
                return;
            for (int j = 0; j < len; j++)
                var[j] = str[start + j];
            var[len] = '\0';

            char *val = get_env_value(var, envp);
            if (val)
                ft_putstr_fd(val, 1);
            free(var);
        }
        else
        {
            ft_putchar_fd(str[i], 1);
            i++;
        }
    }
}

/*
int main(int ac, char **av, char **envp)
{
    ft_echo(av[1], envp);
    return (0);
}*/