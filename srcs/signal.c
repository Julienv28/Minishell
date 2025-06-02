#include "../includes/minishell.h"

void	set_signal_action(void)
{
	signal(SIGINT, handler_sigint); // Ctrl+C : annule readline
	signal(SIGQUIT, SIG_IGN);       // Ctrl+\ : ignoré comme bash
}
void heredoc_sigint_handler(int sig)
{
    (void)sig;
    write(1, "\n", 1);
    g_exit_status = 130;
}

// handler pour SIGINT (Ctrl+C) dans readline uniquement
void handler_sigint(int sig)
{
    (void)sig;
    write(1, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
    g_exit_status = 130;
}
