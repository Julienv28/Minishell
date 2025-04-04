/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 16:18:50 by juvitry           #+#    #+#             */
/*   Updated: 2025/01/14 15:59:07 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// colle 2 strings et en free une des 2 pour s'assurer de la bonne gestion de 
// la memoire
static char	*ft_free(char *buffer, char *buf)
{
	char	*sub;

	sub = ft_strjoin(buffer, buf);
	if (buffer)
	{
		free (buffer);
		buffer = NULL;
	}
	return (sub);
}

// extraire une ligne du buffer et s'assurer qu'elle s'arrete au \n

static char	*ft_ligne(char *buffer)
{
	char	*line;
	int		i;

	i = 0;
	if (!buffer || !buffer[i])
		return (NULL);
	while (buffer[i] && buffer[i] != '\n')
		i++;
	line = ft_calloc(i + 2, sizeof(char));
	if (!line)
		return (NULL);
	i = 0;
	while (buffer[i] && buffer[i] != '\n')
	{
		line[i] = buffer[i];
		i++;
	}
	if (buffer[i] && buffer[i] == '\n')
		line[i++] = '\n';
	return (line);
}

//Maj du buffer pour un prochain call de la fonction. Retire la ligne lue.
static char	*ft_suite(char *buffer)
{
	char	*line;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (!buffer[i])
	{
		free(buffer);
		return (NULL);
	}
	line = ft_calloc((ft_strlen(buffer) - i + 1), sizeof(char));
	if (!line)
	{
		free (buffer);
		return (NULL);
	}
	i++;
	while (buffer[i])
		line[j++] = buffer[i++];
	free (buffer);
	return (line);
}

// Lis le fichier chunk par chunk jusqu'a rencontrer \n ou \0.
static char	*ft_read_file(int fd, char *res)
{
	char	*buffer;
	int		bit_lu;

	if (!res)
		res = ft_calloc(1, 1);
	buffer = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (buffer == NULL)
		return (NULL);
	bit_lu = 1;
	while (bit_lu > 0)
	{
		bit_lu = read(fd, buffer, BUFFER_SIZE);
		if (bit_lu == -1)
		{
			free(buffer);
			free (res);
			return (NULL);
		}
		buffer[bit_lu] = 0;
		res = ft_free(res, buffer);
		if (ft_strchr(buffer, '\n'))
			break ;
	}
	free (buffer);
	return (res);
}

//Lis une ligne, maj le buffer et retourne la ligne
char	*get_next_line(int fd)
{
	static char	*buffer;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
	{
		if (buffer)
		{
			free (buffer);
			buffer = NULL;
		}
		return (NULL);
	}
	buffer = ft_read_file(fd, buffer);
	if (!buffer)
		return (NULL);
	line = ft_ligne(buffer);
	buffer = ft_suite(buffer);
	return (line);
}

// #include <stdio.h>

// int	main(void)
// {
// 	int		fd;
// 	char	*line;
// 	int		lines;

// 	lines = 1;
// 	fd = open("read_error.txt", O_RDONLY);
// 	while ((line = get_next_line(fd)))
// 		printf("%d->%s\n", lines++, line);
// 	free (line);
// }

// if (!buffer || !line)
// {
// 	if (buffer)
// 		free(buffer);
// 	return (NULL);
// }