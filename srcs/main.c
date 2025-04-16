#include "../includes/minishell.h"

int g_exit_status = 0; // variable globale pour signaux initialisé a 0

int main(int ac, char **av, char **envp)
{
    char *input;
    char **args = NULL;
    char    **envcp;
    t_token *tokens;
    t_com_list *command;
    (void)ac;
    (void)av;
    int mem_fd;

    envcp = ft_env_dup(envp);
    while (1)
    {
        set_signal_action(); // gestion des signaux (SIGINT ET SIGQUIT)
        input = readline(GREEN "minishell$ " RESET); // Demander une saisie
        if (!input)
        {
            ft_putstr_fd("exit\n", STDOUT_FILENO);
            exit(g_exit_status);
        }
        add_history(input);  // Ajouter l'entrée dans l'historique      
        tokens = create_tokens(&input);   // Créer les tokens
        if (!tokens)
        {
            free(tokens);
            continue;           // retourne immédiatement au prompt principal
        }
        free(input);                      // Libérer input après la création des tokenss
        command = tokens_to_cmds(tokens); // Convertir les tokens en commandes
        while (command)
        {
            //printf("Commande : %s, Pipe : %d\n", command->command, command->is_pipe);
            args = split_args(command->command, ' ');
            //Affiche args
            if (args)
            {
                int i = 0;
                while (args[i])
                {
                    printf("args[%d] = %s\n", i, args[i]);
                    i++;
                }
            }
            
            // Appliquer redirection avant execution
            if (command->infile || command->outfile || command->errfile)
                mem_fd = ft_redirection(command);
            // Exécuter la commande
            if (args && is_builting(args[0]) == 0)
                exec_builting(args, &envcp);
            else
                exec_cmd(command, envcp);
            // Rétablir les redirections
            if (command->infile || command->outfile || command->errfile)
                putback_direction(command, mem_fd);
            free_tab(args);
            // if (command->next == NULL)
            //     printf("Fin de la liste des commandes\n");
            command = command->next;
        }
        free_cmd(command);
        free_tokens(tokens); // Libérer les tokens
    }
    ft_freeenvp(envcp);
    return (g_exit_status);
}
