#include "../includes/minishell.h"

// 1. Création de tokens a partir de la ligne de commande
// 2. Identification des types de tokens (arg, cmd, pipe, redirection..)
// 3. Creation des commandes avec le regroupement des tokens
// 4. Detection des pipes et redirections
// 5. Detection des fichier d'entres et sortie (fd)

int main(void)
{
    char *input;
    t_token *tokens;
    t_com_list *command;

    while (1)
    {
        input = readline(GREEN "minishell$ " RESET); // Demander une saisie
        if (!input)
        {
            perror("readline");
            exit_error(); // Quitter proprement en cas d'erreur
            break;
        }
        add_history(input); // Ajouter l'entrée dans l'historique
        if (ft_strncmp(input, "exit", 4) == 0)
        {
            free(input);
            break;
        }
        tokens = create_tokens(&input);   // Créer les tokens
        free(input);                      // Libérer input après la création des tokens
        command = tokens_to_cmds(tokens); // Convertir les tokens en commandes
        while (command)
        {
            printf("Commande : %s, Pipe : %d\n", command->command, command->is_pipe);
            command = command->next;
        }
        free_tokens(tokens); // Libérer les tokens
    }
    return (0);
}