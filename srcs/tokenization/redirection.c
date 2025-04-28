#include "../includes/minishell.h"

// Fonction pour gérer la redirection dans la commande
int handle_redirection(char *str, int *i, t_token **tokens)
{
    int start;
    int type;
    char *word;
    char *symbol;

    // Vérifier les redirections
    type = parse_redirection(str, i);
    if (type)
    {
        symbol = add_symbol(type);       // Créer le symbole de redirection
        add_token(tokens, symbol, type); // Ajouter le token de redirection
        free(symbol);

        if (check_redirection(str, i) == -1)
            return (-1);

        // Sauter les espaces entre > et le fichier
        while (str[*i] == ' ')
            (*i)++;

        start = *i;

        // Récupérer le fichier après la redirection
        while (str[*i] && str[*i] != ' ' && str[*i] != '|' && str[*i] != '<' && str[*i] != '>')
            (*i)++;

        if (*i == start) // Si aucun fichier n'est spécifié
        {
            printf("Erreur : Redirection sans fichier spécifié !\n");
            return (-1);
        }

        word = ft_strndup(str + start, *i - start); // Copier le fichier de redirection
        add_token(tokens, word, ARG);               // Ajouter le fichier comme argument
        free(word);
        return (1); // Redirection traitée
    }
    return (0); // Pas de redirection
}

// Analyser les redirections et avancer l'index
int parse_redirection(char *str, int *i)
{
    if (str[*i] == '<' && str[*i + 1] == '<')
    {
        *i += 2; // Avancer l'index de 2 pour ignorer "<<"
        printf("Détection d'une redirection HEREDOC (<<)\n");
        return (HEREDOC);
    }
    else if (str[*i] == '>' && str[*i + 1] == '>')
    {
        *i += 2;
        printf("Détection d'une redirection en mode APPEND (>>)\n");
        return (APPEND);
    }
    else if (str[*i] == '<')
    {
        (*i)++;
        printf("Détection d'une redirection d'entrée INPUT (<)\n");
        return (INPUT);
    }
    else if (str[*i] == '>')
    {
        (*i)++;
        printf("Détection d'une redirection de sortie TRUNC (>)\n");
        return (TRUNC);
    }
    return (0);
}

// Convertir un type en symbole de redirection
char *add_symbol(int type)
{
    if (type == HEREDOC)
        return (ft_strdup("<<"));
    else if (type == APPEND)
        return (ft_strdup(">>"));
    else if (type == INPUT)
        return (ft_strdup("<"));
    else if (type == TRUNC)
        return (ft_strdup(">"));
    return (NULL);
}

int ft_redirection(t_com_list *command)
{
    int fd = -1;
    int mem_fd = -1;
    t_file_list *tmp = command->all_outfilles;

    // Redirection entrée (STDIN)
    if (command->infile)
    {
        mem_fd = dup(STDIN_FILENO);
        if (mem_fd == -1)
        {
            perror("Erreur lors de la sauvegarde de STDIN");
            return (-1);
        }
        fd = open_file_cmd(command->infile);
        if (fd == -1)
        {
            close(mem_fd);
            return (-1);
        }
        if (dup2(fd, STDIN_FILENO) == -1)
        {
            perror("Erreur avec dup2 pour la redirection d'entrée");
            close(fd);
            close(mem_fd);
            return (-1);
        }
        close(fd);
    }
    // Redirection sortie (STDOUT)
    else if (command->outfile)
    {
        mem_fd = dup(STDOUT_FILENO);
        if (mem_fd == -1)
        {
            perror("Erreur lors de la sauvegarde de STDOUT");
            return (-1);
        }

        // Redirection vers plusieurs fichiers successifs si besoin
        while (tmp)
        {
            fd = open_outfile(tmp->filename, command->flag_out);
            printf("Redirection de sortie vers le fichier: %s\n", command->outfile);
            if (fd != -1)
            {
                if (dup2(fd, STDOUT_FILENO) == -1)
                {
                    perror("Erreur avec dup2 pour un fichier de sortie");
                    close(fd);
                    close(mem_fd);
                    return (-1);
                }
                close(fd);
            }
            tmp = tmp->next;
        }

        // Redirection finale vers outfile principal
        fd = open_outfile(command->outfile, command->flag_out);
        if (fd == -1)
        {
            close(mem_fd);
            return (-1);
        }
        printf("Redirection de sortie vers le fichier: %s\n", command->outfile);
        if (dup2(fd, STDOUT_FILENO) == -1)
        {
            perror("Erreur avec dup2 pour outfile principal");
            close(fd);
            close(mem_fd);
            return (-1);
        }
        close(fd);
    }
    // Redirection erreur (STDERR)
    else if (command->errfile)
    {
        mem_fd = dup(STDERR_FILENO);
        if (mem_fd == -1)
        {
            perror("Erreur lors de la sauvegarde de STDERR");
            return (-1);
        }
        fd = open_errfile(command->errfile);
        if (fd == -1)
        {
            close(mem_fd);
            return (-1);
        }
        if (dup2(fd, STDERR_FILENO) == -1)
        {
            perror("Erreur avec dup2 pour la redirection d'erreur");
            close(fd);
            close(mem_fd);
            return (-1);
        }
        close(fd);
    }

    return mem_fd; // on retourne mem_fd pour restaurer plus tard
}

void putback_direction(t_com_list *command, int mem_fd)
{
    if (mem_fd == -1)
    {
        perror("Descripteur de fichier invalide (mem_fd) dans putback_direction");
        return;
    }

    // Rétablir la redirection de l'entrée (STDIN)
    if (command->infile)
        dup2(mem_fd, STDIN_FILENO);
    // Rétablir la redirection de la sortie (STDOUT)
    else if (command->outfile)
        dup2(mem_fd, STDOUT_FILENO);
    // Rétablir la redirection des erreurs (STDERR)
    else if (command->errfile)
        dup2(mem_fd, STDERR_FILENO);
    close(mem_fd);
}
