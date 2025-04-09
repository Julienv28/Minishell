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
        symbol = add_symbol(type);
        printf("Ajout d'une redirection : %s\n", symbol);
        add_token(tokens, symbol, type); // Ajout du token de redirection
        free(symbol);
        if (check_redirection(str, i) == -1)
            return (-1);
        // Récupérer le fichier de redirection
        start = *i;
        while (str[*i] && str[*i] != ' ' && str[*i] != '|' && str[*i] != '<' && str[*i] != '>')
            (*i)++;

        if (*i == start) // Vérification d'un fichier manquant après la redirection
        {
            printf("Erreur : Redirection sans fichier spécifié !\n");
            return (-1);
        }
        word = ft_strndup(str + start, *i - start);
        printf("Ajout du fichier de redirection : %s\n", word);
        add_token(tokens, word, ARG); // Ajout du fichier comme argument
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

// Rediriger les entrées et sortie d'une commande
int ft_redirection(t_com_list *command)
{
    int fd;
    int mem_fd;
    int constante;

    fd = -1;
    mem_fd = -1;
    constante = -1;
    // Gestion de l'entrée (STDIN)
    if (command->infile)
    {
        constante = STDIN_FILENO;
        mem_fd = dup(STDIN_FILENO);
        printf("Redirection d'entrée vers fichier: %s\n", command->infile); // Débogage
        fd = open_file_cmd(command->infile);
    }
    // Gestion de la sortie (STDOUT)
    else if (command->outfile)
    {
        constante = STDOUT_FILENO;
        mem_fd = dup(STDOUT_FILENO);
        printf("Redirection de sortie vers fichier %s\n", command->outfile);
        fd = open_outfile(command->outfile, command->flag_out);
    }
    // Gestion des erreurs (STDERR)
    else if (command->errfile)
    {
        constante = STDERR_FILENO;
        mem_fd = dup(STDERR_FILENO);
        printf("Redirection d'erreur vers fichier: %s\n", command->errfile); // Débogage
        fd = open_errfile(command->errfile);
    }
    if (fd != -1)
    {
        if (dup2(fd, constante) == -1) // transforme fd en une copie de la sortie
        {
            perror("Erreur avec dup2 pour l'entrée");
            close(fd);
            return (-1);
        }
        close(fd);
    }
    return (mem_fd);
}

void putback_direction(t_com_list *command, int mem_fd)
{
    if (command->infile)
        dup2(mem_fd, STDIN_FILENO);
    else if (command->outfile)
        dup2(mem_fd, STDOUT_FILENO);
    else if (command->errfile)
        dup2(mem_fd, STDERR_FILENO);
}