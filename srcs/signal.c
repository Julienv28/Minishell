#include "../includes/minishell.h"

// Ctrl+C (SIGINT) : Interrompt la commande en cours mais ne ferme pas le shell.
// Ctrl +D(EOF) : Quitte proprement le shell.
// Ctrl +\ (SIGQUIT) : Ignoré ou utilisé pour afficher un dump mémoire.Fonctions utiles

void signal_handler(int sig)
{
    // exit_status = variable globale qui suit la valeur de sortie des signaux
    if (sig == SIGINT) // Si Ctrl+C
    {
        ft_putstr_fd("\n", STDOUT_FILENO);
        rl_on_new_line();       // Dit à readline que tu es sur une nouvelle ligne. Donc création nouvelle ligne"
        rl_replace_line("", 0); // Remplace la ligne de commande courante par une chaîne vide (ou autre). Donc Vider la ligne en cours
        rl_redisplay();         // Force le rafraîchissement du prompt.
        g_exit_status = 130;    // Code de sortie pour une interruption par Ctrl+C
    }
    // Ajouter signal erreur quand prompt secondaire ouvert '>' et Ctrl+C

    else if (sig == SIGQUIT) // (Si Ctrl +\)
    {
        ft_putstr_fd("Quit: 3\n", STDOUT_FILENO); // message par defaut
        g_exit_status = 131;                      // 131 = interruption par (Ctrl +\)
    }
}

void set_signal_action(void)
{
    if (signal(SIGINT, signal_handler) == SIG_ERR)
        exit_error();
    else if (signal(SIGQUIT, signal_handler) == SIG_ERR)
        exit_error();
}