#include "../includes/minishell.h"

void	set_signal_action(void)
{
	signal(SIGINT, handler_sigint);
	signal(SIGQUIT, SIG_IGN);
}

void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	close(STDIN_FILENO);
	g_exit_status = 130;
}

void	handler_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_exit_status = 130;
}

void	handler_sigint_prompt(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	close(0);
	g_exit_status = 130;
}
