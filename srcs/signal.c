#include "../includes/minishell.h"

// Ctrl+C (SIGINT) : Interrompt la commande en cours mais ne ferme pas le shell.
// Ctrl +D(EOF) : Quitte proprement le shell.
// Ctrl +\ (SIGQUIT) : Ignoré ou utilisé pour afficher un dump mémoire.Fonctions utiles

void	set_signal_action(void)
{
    // if (signal(SIGINT, signal_handler) == SIG_ERR)
    //     exit_error();
    // else if (signal(SIGQUIT, signal_handler) == SIG_ERR)
    //     exit_error();
	signal(SIGINT, handler_sigint); // Ctrl+C : annule readline
	signal(SIGQUIT, SIG_IGN);       // Ctrl+\ : ignoré comme bash
}

// handler pour SIGINT (Ctrl+C) dans readline uniquement
void	handler_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_exit_status = 130;
}
