#include "../includes/minishell.h"

int g_exit_status = 0;

int main(int ac, char **av, char **envp)
{
	char		*input;
	t_token		*tokens;
	t_com_list	*command;
	t_com_list	*full_list;
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
                    exec_builting(command->args, &command->envcp);
                else if (command->args)
                    exec_cmd(command);
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
    ft_freeenvp(command->envcp);
    return (g_exit_status);
}

// int	main(int ac, char **av, char **envp)
// {
// 	char		*input;
// 	t_com_list	*commands;

// 	(void)ac;
// 	(void)av;
// 	while (1)
// 	{
// 		set_signal_action();
// 		input = readline(GREEN "minishell$" RESET);
// 		if (!input)
// 		{
// 			ft_putstr_fd("exit\n", STDOUT_FILENO);
// 			exit(g_exit_status);
// 		}
// 		add_history(input);
// 		commands = parse_pipes(input);
// 		free(input);
// 		t_com_list	*tmp = commands;
// 		while (tmp)
// 		{
// 			tmp->envcp = ft_env_dup(envp);
// 			tmp = tmp->next;
// 		}
// 		if (commands)
// 		{
// 			execute_cmd(commands);
// 			ft_freeenvp(commands->envcp);
// 			free_cmd(commands);
// 		}
// 	}
// 	return (g_exit_status);
// }
