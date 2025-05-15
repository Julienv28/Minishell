/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oceanepique <oceanepique@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:58:04 by juvitry           #+#    #+#             */
/*   Updated: 2025/05/15 17:24:24 by oceanepique      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
/*
int	open_file(char *av, int i)
{
    int	file;

    file = 0;
    if (i == 0)
        file = open(av, O_WRONLY | O_CREAT | O_APPEND, 0777);
    else if (i == 1)
        file = open(av, O_WRONLY | O_CREAT | O_TRUNC, 0777);
    else if (i == 2)
        file = open(av, O_RDONLY, 0777);
    if (file == -1)
        exit_error();
    return (file);
}

int	simplified_gnl(char **line)
{
    char	*buffer;
    int		i;
    int		r;
    char	c;

    i = 0;
    r = 0;
    buffer = (char *)malloc(1000);
    if (!buffer)
        return (-1);
    r = read(0, &c, 1);
    while (r && c != '\n' && c != '\0')
    {
        if (c != '\n' && c != '\0')
            buffer[i] = c;
        i++;
        r = read(0, &c, 1);
    }
    buffer[i] = '\n';
    buffer[++i] = '\0';
    *line = buffer;
    free(buffer);
    return (r);
}
*/