#include "../includes/minishell.h"

int g_exit_status = 0;

int	has_redirection(t_com_list *cmd)
{
	return (cmd->infile || cmd->outfile || cmd->errfile || cmd->heredoc_fd > 0);
}

int	handle_execution(t_com_list *cmd, char ***envcp, t_redirs *fds)
{
	int	error;

	error = 0;
	if (has_redirection(cmd))
		error = ft_redirection(cmd, &fds->in, &fds->out, &fds->err);
	if (error)
		return (0);
	if (has_pipe(cmd))
	{
		exec_pipes(cmd, *envcp);
		restore_redirections(fds->in, fds->out, fds->err);
		init_redirs(fds);
		return (1);
	}
	execute(cmd, envcp);
	if (has_redirection(cmd))
		restore_redirections(fds->in, fds->out, fds->err);
	init_redirs(fds);
	return (0);
}

int	handle_empty_command(t_com_list *cmd, t_redirs *fds)
{
	int	error;

	if (cmd->command)
		return (0);
	if (!has_redirection(cmd))
		return (1);
	error = ft_redirection(cmd, &fds->in, &fds->out, &fds->err);
	if (error != 0)
		g_exit_status = 1;
	restore_redirections(fds->in, fds->out, fds->err);
	init_redirs(fds);
	return (1);
}

void	execute_commands(t_com_list *cmd, char ***envcp)
{
	t_com_list	*cur;
	t_redirs	fds;

	cur = cmd;
	init_redirs(&fds);
	while (cur)
	{
		if (handle_empty_command(cur, &fds))
		{
			cur = cur->next;
			continue ;
		}
		if (handle_execution(cur, envcp, &fds))
			break ;
		cur = cur->next;
	}
}

void	minishell_loop(char ***envcp)
{
	char		*input;
	t_token		*tokens;
	t_com_list	*commands;

	while (1)
	{
		set_signal_action();
		input = readline(GREEN "minishell$ " RESET);
		if (!input)
			exit_shell(*envcp);
		add_history(input);
		tokens = create_tokens(&input, *envcp);
		if (!tokens)
		{
			free(input);
			continue ;
		}
		if (!handle_null_tokens(tokens, input))
		{
			free_tokens(tokens);
			continue ;
		}
		free(input);
		commands = tokens_to_cmds(tokens, *envcp);
		free_tokens(tokens);
		if (!commands)
			continue ;
		execute_commands(commands, envcp);
		free_cmd(commands);
	}
}

int	main(int ac, char **av, char **envp)
{
	char	**envcp;

	(void)ac;
	(void)av;
	if (!check_isatty())
		return (0);
	envcp = ft_env_dup(envp);
	init_signals();
	minishell_loop(&envcp);
	ft_freeenvp(envcp);
	rl_clear_history();
	return (g_exit_status);
}

/*
int main(int ac, char **av, char **envp)
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
    if (isatty(1) == 0)
    {
        write(1, "you can't pipe a minishell\n", 27);
        return (0);
    }
    if (isatty(0) == 0)
    {
        write(1, "you can't pipe a minishell\n", 27);
        return (0);
    }
    signal(SIGTSTP, SIG_IGN);
    signal(SIGINT, handler_sigint);
    signal(SIGQUIT, SIG_IGN);
    while (1)
    {
        set_signal_action();
        input = readline(GREEN "minishell$ " RESET);
        if (!input)
        {
            ft_putstr_fd("exit\n", STDOUT_FILENO);
            rl_clear_history();
            ft_freeenvp(envcp);
			exit(g_exit_status);
		}
        add_history(input);
        tokens = create_tokens(&input, envcp);
        if (tokens == NULL)
        {
            if (input && is_blank_line(input))
            {
                g_exit_status = 0;
                free(input);
                free_tokens(tokens);
                continue;
            }
            else
            {
                g_exit_status = 2;
                free(input);
                free_tokens(tokens);
                continue;
            }
        }
        free(input);
        if (!tokens)
        {
            free_tokens(tokens);
            continue ;
        }
        command = tokens_to_cmds(tokens, envcp);
        if (!command)
        {
            free_tokens(tokens);
            continue ;
        }
        free_tokens(tokens);
        t_com_list *start = command;
        while (command)
        {
            has_redir_error = 0;
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
                continue ;
            }
            if (command->infile || command->outfile || command->errfile || command->heredoc_fd > 0)
                has_redir_error = ft_redirection(command, &mem_fd_in, &mem_fd_out, &mem_fd_err);
            if (has_redir_error)
            {
                command = command->next;
                continue ;
            }
            if (has_pipe(command))
            {
                exec_pipes(command, envcp);
                restore_redirections(mem_fd_in, mem_fd_out, mem_fd_err);
                mem_fd_in = mem_fd_out = mem_fd_err = -1;
                break ;
            }
            else
                execute(command, &envcp);
            if ((command->infile || command->outfile || command->errfile || command->heredoc_fd > 0) && has_redir_error >= 0)
            {
                restore_redirections(mem_fd_in, mem_fd_out, mem_fd_err);
                mem_fd_in = mem_fd_out = mem_fd_err = -1;
            }
            command = command->next;
        }
        free_cmd(start);
    }
    
    ft_freeenvp(envcp);
    rl_clear_history();
    return (g_exit_status);
}*/
