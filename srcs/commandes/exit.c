/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juvitry <juvitry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 16:32:49 by opique            #+#    #+#             */
/*   Updated: 2025/06/16 13:45:31 by juvitry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_isnumber(char *str)
{
	int	i;

	i = 0;
	if (!str || str[0] == '\0')
		return (0);
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	nbr_of_sign(char *args)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (args[i])
	{
		if (args[i] == '-' || args[i] == '+')
			count++;
		i++;
	}
	return (count);
}

unsigned long long	ft_atoull(const char *str)
{
	unsigned long long	result;
	int					i;

	result = 0;
	i = 0;
	while (str[i] && ft_isdigit(str[i]))
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result);
}

int	is_valid_numeric_argument(char *str)
{
	int					sign;
	unsigned long long	value;

	if (!str)
		return (0);
	sign = 1;
	if (nbr_of_sign(str) > 1 || !ft_isnumber(str))
		return (0);
	if (str[0] == '-')
		sign = -1;
	if (str[0] == '-' || str[0] == '+')
		str++;
	value = ft_atoull(str);
	if ((sign == 1 && value > LLONG_MAX)
		|| (sign == -1 && value > (unsigned long long)LLONG_MAX + 1))
		return (0);
	return (1);
}

int	ft_exit(char **args, t_com *cmd, t_msh *msh)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	if (i == 1)
		cleanup_and_exit(0, cmd);
	else if (!is_valid_numeric_argument(args[1]))
	{
		print_exit_error(args[1], ": numeric argument required", STDERR_FILENO);
		exit(255);
	}
	else if (i > 2)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		msh->ex_status = 1;
		return (msh->ex_status);
	}
	cleanup_and_exit(ft_atoi(args[1]), cmd);
	return (msh->ex_status);
}
