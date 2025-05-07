#include "../includes/minishell.h"

int g_exit_status = 0;
/*
int main(int ac, char **av, char **envp)
{
    char		*input;
    t_token		*tokens;
    t_com_list	*command;
    int			mem_fd;

    (void)ac;
    (void)av;
    command->envcp = ft_env_dup(envp);
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
            continue ;
        }
        free(input);
        command = tokens_to_cmds(tokens);
        while (command)
        {
            command->args = split_args(command->command, ' ');
            if (command->args)
                replace_exit_and_env_status(command->args);

            // Appliquer la redirection
            if (command->infile || command->outfile || command->errfile)
                mem_fd = ft_redirection(command);

            if (mem_fd == -1)
            {
                // erreur de redirection : on ne fait rien, on passe à la prochaine commande
                free_tab(command->args);
                command = command->next;
                continue;
            }

            // sinon on exécute
            else
            {
                if (command->args && is_builting(command->args[0]) == 0)
                    exec_builting(command->args, &envcp);
                else if (command->args)
                    exec_cmd(command, envcp);
            }

            // remettre les redirections normales
            if (mem_fd >= 0 && (command->infile || command->outfile || command->errfile))
                putback_direction(command, mem_fd);

            free_tab(command->args);
            command = command->next;
        }
        free_cmd(command);
        free_tokens(tokens);
    }
    ft_freeenvp(envcp);
    return (g_exit_status);
}*/

int main(int ac, char **av, char **envp)
{
    char *input;
    char **args = NULL;
    char **envcp;
    t_token *tokens;
    t_com_list *command;
    int mem_fd_in = -1, mem_fd_out = -1, mem_fd_err = -1;
    int has_redir_error = 0;

    (void)ac;
    (void)av;

    envcp = ft_env_dup(envp);

    while (1)
    {
        set_signal_action();
        input = readline(GREEN "minishell$ " RESET);
        if (!input)
        {
            ft_putstr_fd("exit\n", STDOUT_FILENO);
            exit(g_exit_status);
        }

        add_history(input);
        tokens = create_tokens(&input);
        free(input);

        if (!tokens)
        {
            free_tokens(tokens);
            continue;
        }
        command = tokens_to_cmds(tokens);
        if (!command)
            printf("Aucune commande générée\n");
        while (command)
        {
            has_redir_error = 0;

            // Si aucune commande, mais redirection présente
            if (command->command == NULL)
            {
                if (command->infile || command->outfile || command->errfile)
                {
                    has_redir_error = ft_redirection(command, &mem_fd_in, &mem_fd_out, &mem_fd_err);
                    restore_redirections(mem_fd_in, mem_fd_out, mem_fd_err);
                    mem_fd_in = mem_fd_err = -1;
                }
                command = command->next;
                continue;
            }

            // Sécurité : ne pas appeler split_args sur NULL
            if (command->command)
                args = split_args(command->command, ' ');
            else
                args = NULL;

            if (args)
                replace_exit_and_env_status(args, envcp);

            // Appliquer les redirections
            if (command->infile || command->outfile || command->errfile)
                has_redir_error = ft_redirection(command, &mem_fd_in, &mem_fd_out, &mem_fd_err);

            if (has_redir_error)
            {
                free_tab(args);
                command = command->next;
                continue;
            }
            int pipes_count = parse_pipes(args);
            if (pipes_count > 0)
                pipes_manager(command, pipes_count, args, envcp);
            else
            {
                if (args && args[0] && is_builting(args[0]) == 0)
                    exec_builting(args, &envcp);
                else if (args && args[0])
                    exec_cmd(command, envcp);
            }

            // Restauration des redirections
            if ((command->infile || command->outfile || command->errfile) && has_redir_error >= 0)
            {
                restore_redirections(mem_fd_in, mem_fd_out, mem_fd_err);
                mem_fd_in = mem_fd_out = mem_fd_err = -1;
            }

            free_tab(args);
            command = command->next;
        }
    }
    ft_freeenvp(envcp);
    return g_exit_status;
}
