#include "../includes/minishell.h"

int g_exit_status = 0;

int main(int ac, char **av, char **envp)
{
    char *input;
    char **args = NULL;
    char **envcp;
    t_token *tokens;
    t_com_list *command;
    (void)ac;
    (void)av;
    int mem_fd;

    envcp = ft_env_dup(envp);
    while (1)
    {
        mem_fd = -2; // <- Très important pour bien contrôler l'état
        set_signal_action();
        input = readline(GREEN "minishell$ " RESET);
        if (!input)
        {
            ft_putstr_fd("exit\n", STDOUT_FILENO);
            exit(g_exit_status);
        }
        add_history(input);
        tokens = create_tokens(&input);
        if (!tokens)
        {
            free(tokens);
            continue;
        }
        free(input);
        command = tokens_to_cmds(tokens);
        while (command)
        {
            args = split_args(command->command, ' ');
            if (args)
                replace_exit_and_env_status(args);

            // Appliquer la redirection
            if (command->infile || command->outfile || command->errfile)
                mem_fd = ft_redirection(command);

            if (mem_fd == -1)
            {
                // erreur de redirection : on ne fait rien, on passe à la prochaine commande
                free_tab(args);
                command = command->next;
                continue;
            }

            //On voit les pipes
            int pipes_count = parse_pipes(args);
            if (pipes_count != 0)
                pipes_manager(command, pipes_count, args, envcp);

            // sinon on exécute
            else
            {
                if (args && is_builting(args[0]) == 0)
                    exec_builting(args, &envcp);
                else if (args)
                    exec_cmd(command, envcp);
            }

            // remettre les redirections normales
            if (mem_fd >= 0 && (command->infile || command->outfile || command->errfile))
                putback_direction(command, mem_fd);

            free_tab(args);
            command = command->next;
        }
        free_cmd(command);
        free_tokens(tokens);
    }
    ft_freeenvp(envcp);
    return (g_exit_status);
}
