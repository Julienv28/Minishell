#include "../includes/minishell.h"

void ft_env(char **envcp)
{
    int i;

    i = 0;
    while (envcp && envcp[i])
    {
        printf("%s\n", envcp[i]);
        i++;
    }
}

