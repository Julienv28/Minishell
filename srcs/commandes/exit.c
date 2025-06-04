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

// Convertit une string en unsigned long long (sans gérer les signes)
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

// Valide que str est un entier valide et dans la plage de long long
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

void	cleanup_and_exit(int code, t_com_list *cmd)
{
	free_cmd(cmd);
	rl_clear_history();
	exit(code);
}

int	ft_exit(char **args, int in_child, t_com_list *cmd)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	if (in_child)
		printf("exit\n");
	if (i == 1)
		cleanup_and_exit(0, cmd);
	else if (!is_valid_numeric_argument(args[1]))
	{
		printf("exit\n");
		printf("minishell: exit: %s: numeric argument required\n", args[1]);
		exit(255);
	}
	else if (i > 2)
	{
		printf("exit\n");
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		g_exit_status = 1;
		return (g_exit_status);
	}
	cleanup_and_exit(ft_atoi(args[1]), cmd);
	return (g_exit_status);
}
