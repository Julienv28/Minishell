#include "../includes/minishell.h"

int	check_redirection(char *str, int *i)
{
    // Récupérer le fichier qui suit la redirection
	while (str[*i] == ' ') // Ignorer les espaces après la redirection
		(*i)++;

    // Vérifier si un fichier suit bien la redirection
	if (!str[*i] || str[*i] == '|' || str[*i] == '<' || str[*i] == '>')
	{
		if (!str[*i])
			printf("Erreur : syntax error near unexpected token newline'\n");
		else
			printf("Erreur : syntax error near unexpected token %c'\n", str[*i]);
		return (-1);
	}
	return (0);
}
int check_pipe(char *str, int i)
{
    int j;
    // Vérifier si le pipe est en début de commande, suivi d'un autre pipe, ou à la fin
    if (str[i + 1] == '\0' || i == 0)
    {
        printf("Erreur : syntax error near unexpected token `%c'\n", str[i]);
        return (-1);
    }
    // Vérifier si le caractère précédent est aussi un pipe
    if (str[i - 1] == '|')
    {
        printf("Erreur : syntax error near unexpected token `%c'\n", str[i]);
        return (-1);
    }
    // Vérifier si plusieurs pipes consécutifs avec espaces
    j = i + 1;
    while (str[j] == ' ')
        j++;
    if (str[j] == '|')
    {
        printf("Erreur : syntax error near unexpected token `%c'\n", str[j]);
        return (-1);
    }
    return (0);
}
