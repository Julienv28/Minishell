#include "../includes/minishell.h"

// Ctrl+C (SIGINT) : Interrompt la commande en cours mais ne ferme pas le shell.
// Ctrl +D(EOF) : Quitte proprement le shell.
// Ctrl +\ (SIGQUIT) : Ignoré ou utilisé pour afficher un dump mémoire.Fonctions utiles

void signal_handler(int sig)
{
    if (sig == SIGINT) // Si Ctrl+C
    {
        if (g_exit_status == 1) // cmd
        {
            g_exit_status = 130;
            rl_replace_line("", 0);
            write(1, "\n", 1); // Assure-toi que le retour à la ligne est bien fait
            rl_on_new_line();  // Met à jour readline pour qu'il commence une nouvelle ligne
            rl_redisplay();
        }
        else
        {
            g_exit_status = 130;    // Code de sortie pour une interruption par Ctrl+C
            rl_replace_line("", 0); // Remplace la ligne de commande courante par une chaîne vide (ou autre). Donc Vider la ligne en cours
            write(1, "\n", 1);
            rl_on_new_line(); // Dit à readline que tu es sur une nouvelle ligne. Donc création nouvelle ligne"
            rl_redisplay();   // Force le rafraîchissement du prompt.
        }
    }

    // Ajouter signal erreur quand prompt secondaire ouvert '>' et Ctrl+C
    else if (sig == SIGQUIT) // (Si Ctrl +\)
    {
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