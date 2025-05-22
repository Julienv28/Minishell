#include "../includes/minishell.h"

int ft_isnumber(char *str)
{
    int i;

    i = 0;
    if (!str || str[0] == '\0')
        return (0);
    if (str[i] == '-' || str[i] == '+')
        i++;
    while (str[i])
    {
        if (!ft_isdigit(str[i])) // ici on passe bien un caractère
            return (0);
        i++;
    }
    return (1);
}

int nbr_of_sign(char *args)
{
    int count;
    int i;

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
unsigned long long ft_atoull(const char *str)
{
    unsigned long long result = 0;
    int i = 0;

    while (str[i] && ft_isdigit(str[i]))
    {
        result = result * 10 + (str[i] - '0');
        i++;
    }
    return result;
}
// Valide que str est un entier valide et dans la plage de long long
int is_valid_numeric_argument(char *str)
{
    int sign = 1;
    unsigned long long value;

    if (!str)
        return (0);

    // Trop de signes = non conforme
    if (nbr_of_sign(str) > 1 || !ft_isnumber(str))
        return (0);

    // Gère le signe
    if (str[0] == '-')
        sign = -1;
    if (str[0] == '-' || str[0] == '+')
        str++;

    value = ft_atoull(str);

    // Vérifie si le nombre dépasse la limite selon le signe
    if ((sign == 1 && value > LLONG_MAX)
        || (sign == -1 && value > (unsigned long long)LLONG_MAX + 1))
        return (0);

    return (1);
}


// plus de 1 - ou + = exit: ... : numeric argument required
// plus de 1 arg = too many argument meme si - ou + sauf si pas que des chiffre
void ft_exit(char **args, int in_child)
{
    int i;

    i = 0;
    // Compte le nombre d'arguments
    while (args[i])
        i++;
    if (in_child)
    {
        printf("exit\n");
        //exit(0);
    }
    
    if (i == 1) // Si av = 1 donc juste exit quitter avec le code 0 (succes)
    {
        //printf("exit\n");
        exit(0);
    }
    else if (!is_valid_numeric_argument(args[1])) // Si 1e arg n'est pas un entier, on affiche un message d’erreur et on quitte avec le code 255 (comportement standard).
    {
        printf("exit\n");
        ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
        ft_putstr_fd(args[1], STDERR_FILENO);
        ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
        exit(255);
    }
    else if (i > 2)
    {
        printf("exit\n");
        ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
        return; // ne quitte pas, reste dans le shell
    }
    //printf("exit\n");
    exit(ft_atoi(args[1]));
}