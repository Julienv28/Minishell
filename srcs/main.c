#include "../includes/minishell.h"

// 1. Création de tokens a partir de la ligne de commande
// 2. Identification des types de tokens (arg, cmd, pipe, redirection..)
// 3. Creation des commandes avec le regroupement des tokens
// 4. Detection des pipes et redirections
// 5. Detection des fichier d'entres et sortie (fd)

// EXPORT
// Sans argument → affiche toutes les variables d’environnement triées par ordre alpha (à la declare -x VAR="val").
// Avec VAR=value → ajoute/modifie la variable d’environnement.
// Avec VAR seul → crée une var avec valeur vide si elle n’existe pas.

int main(int ac, char **av, char **envp)
{
    char *input;
    char **args = NULL;
    t_token *tokens;
    t_com_list *command;
    (void)ac;
    (void)av;
    int mem_fd;

    while (1)
    {
        input = readline(GREEN "minishell$ " RESET); // Demander une saisie
        if (!input)
        {
            perror("readline");
            exit_error(); // Quitter proprement en cas d'erreur
            break;
        }
        add_history(input);               // Ajouter l'entrée dans l'historique
        tokens = create_tokens(&input);   // Créer les tokens
        free(input);                      // Libérer input après la création des tokenss
        command = tokens_to_cmds(tokens); // Convertir les tokens en commandes
        while (command)
        {
            printf("Commande : %s, Pipe : %d\n", command->command, command->is_pipe);
            args = split_args(command->command, ' ');
            // Appliquer redirection avant execution
            if (command->infile || command->outfile || command->errfile)
            {
                printf("Application redirection cmd: %s\n", command->command);
                printf("Flag out: %d\n", command->flag_out);
                mem_fd = ft_redirection(command);
            }
            // Exécuter la commande
            if (args && is_builting(args[0]) == 0)
                exec_builting(args, envp);
            else
                exec_cmd(command, envp);
            // Rétablir les redirections
            if (command->infile || command->outfile || command->errfile)
                putback_direction(command, mem_fd);
            free_tab(args);
            if (command->next == NULL)
                printf("Fin de la liste des commandes\n");
            command = command->next;
        }
        free_tokens(tokens); // Libérer les tokens
    }
    return (0);
}