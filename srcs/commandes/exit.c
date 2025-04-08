#include "../includes/minishell.h"

int ft_isnumber(char *str)
{
    int i;

    i = 0;
    if (!str || str[0] == '\0')
        return (0);
    while (str[i])
    {
        if (!ft_isdigit(str[i])) // ici on passe bien un caractère
            return (0);
        i++;
    }
    return (1);
}

void ft_exit(char **args)
{
    int i;

    i = 0;
    // Compte le nombre d'arguments
    while (args[i])
        i++;
    if (i == 1) // Si av = 1 donc juste exit quitter avec le code 0 (succes)
    {
        printf("exit\n");
        exit(0);
    }
    if (!ft_isnumber(args[1])) // Si 1e arg n'est pas un entier, on affiche un message d’erreur et on quitte avec le code 255 (comportement standard).
    {
        printf("exit\n");
        ft_putstr_fd("minishell: exit: numeric argument required\n", STDERR_FILENO);
        exit(255);
    }
    if (i > 2)
    {
        printf("exit\n");
        ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
        return; // ne quitte pas, reste dans le shell
    }
    printf("exit\n");
    // printf("-> devrait quitter avec code : %d\n", ft_atoi(args[1]));
    exit(ft_atoi(args[1]));
}