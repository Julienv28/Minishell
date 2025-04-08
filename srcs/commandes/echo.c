/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oceanepique <oceanepique@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:42:01 by juvitry           #+#    #+#             */
/*   Updated: 2025/04/07 10:31:12 by oceanepique      ###   ########.fr       */
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

void ft_echo(char *str, char **envp)
{
    int i;
    int j;
    char *env_str;

    env_str = NULL;
    i = 0;
    while (str[i])
    {
        if (str[1] == '\"' && str[i] == '$' && ft_isupper(str[i + 1]) == 1)
        {
            j = 0;
            while (str[i] != ' ')
            {
                str[i] = env_str[j];
                i++;
                j++;
            }
            j = 0;
            while (envp[j])
            {
                if (ft_strncmp(env_str, envp[j], ft_strlen(env_str)) == 0)
                {
                    ft_putstr_fd(envp[j], 1);
                    break;
                }
                j++;
            }
            ft_putstr_fd(env_str, 1);
        }
        else
            ft_putchar_fd(str[i], 1);
    }
}

/*
int main(int ac, char **av, char **envp)
{
    ft_echo(av[1], envp);
    return (0);
}*/