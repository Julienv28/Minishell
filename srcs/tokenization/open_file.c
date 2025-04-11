#include "../includes/minishell.h"

// Ouvrir le fichiers d'entrée d'une commande
int open_file_cmd(char *infile)
{
    int fd;

    fd = open(infile, O_RDONLY);
    if (fd == -1)
    {
        perror("Erreur d'ouverture cmd du fichier d'entrée");
        return (-1); // Retourner -1 en cas d'erreur
    }
    return (fd);
}

// Ouvrir le fichiers de sortie d'une commande
int open_outfile(char *outfile, int append)
{
    int fd;

    if (!outfile || !*outfile) // Vérifier si le fichier est valide
    {
        ft_putstr_fd("Erreur : Le fichier de sortie est invalide.\n", STDERR_FILENO);
        return (-1);
    }
    if (append)
        fd = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0644); // O_APPEND pour ajouter à la fin
    else
        fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644); // O_TRUNC pour écraser le fichier
    if (fd == -1)
    {
        perror("Erreur d'ouverture du fichier de sortie");
        return (-1); // Retourner -1 en cas d'erreur
    }
    // printf("fichier %s ouvert\n", outfile);
    return (fd);
}

// Ouvrir le fichiers d'erreur d'une commande
int open_errfile(char *errfile)
{
    int fd;

    fd = open(errfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("Erreur d'ouverture du fichier d'entrée");
        return (-1); // Retourner -1 en cas d'erreur
    }
    return (fd);
}