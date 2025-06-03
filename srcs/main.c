#include "../includes/minishell.h"

int g_exit_status = 0;

int	has_pipe(t_com_list *command)
{
	while (command)
	{
		if (command->is_pipe)
			return (1);
		command = command->next;
	}
	return (0);
}

int	main(int ac, char **av, char **envp)
{
	char		*input;
	char		**envcp;
	t_token		*tokens;
	t_com_list	*command;
    int mem_fd_in = -1, mem_fd_out = -1, mem_fd_err = -1;
    int has_redir_error = 0;

    (void)ac;
    (void)av;
    envcp = ft_env_dup(envp);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGINT, handler_sigint); // pour readline seulement
    signal(SIGQUIT, SIG_IGN);       // on ignore Ctrl+
    while (1)
    {
        set_signal_action();
        input = readline(GREEN "minishell$ " RESET);
        if (!input)
        {
            printf("DEBUG: readline a retourné NULL, on va quitter\n");
            ft_putstr_fd("exit\n", STDOUT_FILENO);
			exit(g_exit_status);
		}
        add_history(input);
        tokens = create_tokens(&input, envcp);
        if (tokens == NULL)
           g_exit_status = 2;
        free(input);
        if (!tokens)
        {
            free_tokens(tokens);
            continue;
        }
        command = tokens_to_cmds(tokens, envcp);
        t_com_list *start = command;
        while (command)
        {
            has_redir_error = 0;

            // Si aucune commande, mais redirection présente
            if (command->command == NULL)
            {
                if (command->infile || command->outfile || command->errfile || command->heredoc_fd > 0)
                {
                    has_redir_error = ft_redirection(command, &mem_fd_in, &mem_fd_out, &mem_fd_err);
                    if (has_redir_error != 0)
                        g_exit_status = 1;
                    restore_redirections(mem_fd_in, mem_fd_out, mem_fd_err);
                    mem_fd_in = mem_fd_err = -1;
                }
                command = command->next;
                continue;
            }

            // Appliquer les redirections
            if (command->infile || command->outfile || command->errfile || command->heredoc_fd > 0)
            {
                printf("redirection detecter\n");
                has_redir_error = ft_redirection(command, &mem_fd_in, &mem_fd_out, &mem_fd_err);
            }

            if (has_redir_error)
            {
                command = command->next;
                continue;
            }
            if (has_pipe(command))
            {
                exec_pipes(command, envcp);
                restore_redirections(mem_fd_in, mem_fd_out, mem_fd_err);
                mem_fd_in = mem_fd_out = mem_fd_err = -1;
                printf("DEBUG: g_exit_status après exec_pipes = %d\n", g_exit_status);
                free_tokens(tokens);
                free_cmd(start);
                break ;
            }
            else
            {
                execute(command, &envcp);
                printf("DEBUG: g_exit_status après execute = %d\n", g_exit_status);
            }
            // Restauration des redirections
            if ((command->infile || command->outfile || command->errfile || command->heredoc_fd > 0) && has_redir_error >= 0)
            {
                restore_redirections(mem_fd_in, mem_fd_out, mem_fd_err);
                mem_fd_in = mem_fd_out = mem_fd_err = -1;
            }
            command = command->next;
        }
            free_tokens(tokens);
            free_cmd(start);
    }
    ft_freeenvp(envcp);
    return (g_exit_status);
}
