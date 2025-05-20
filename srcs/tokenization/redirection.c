#include "../includes/minishell.h"

// Fonction pour gérer la redirection dans la commande
int handle_redirection(char *str, int *i, t_token **tokens, char **envcp)
{
    int start;
    int type;
    char *word;
    char *symbol;
    char *expanded;
    char *cleaned;

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
        expanded = replace_all_variables(word, envcp, 0);
        cleaned = remove_quotes_or_slash(expanded);
        free(expanded);
        word = cleaned;
        add_token(tokens, word, ARG); // Ajouter le fichier comme argument
        free(word);
        return (1); // Redirection traitée
    }
    return (0); // Pas de redirection
}

// Analyser les redirections et avancer l'index
int parse_redirection(char *str, int *i)
{
    if (str[*i] == '2' && str[*i + 1] == '>' && str[*i + 2] == '>')
    {
        (*i) += 3;
        return (ERR_REDIR);
    }
    else if (str[*i] == '2' && str[*i + 1] == '>')
    {
        (*i) += 2;
        return (ERR_REDIR);
    }
    if (str[*i] == '<' && str[*i + 1] == '<')
    {
        *i += 2; // Avancer l'index de 2 pour ignorer "<<"
        // printf("Détection d'une redirection HEREDOC (<<)\n");
        return (HEREDOC);
    }
    else if (str[*i] == '>' && str[*i + 1] == '>')
    {
        *i += 2;
        // printf("Détection d'une redirection en mode APPEND (>>)\n");
        return (APPEND);
    }
    else if (str[*i] == '<')
    {
        (*i)++;
        // printf("Détection d'une redirection d'entrée INPUT (<)\n");
        return (INPUT);
    }
    else if (str[*i] == '>')
    {
        (*i)++;
        // printf("Détection d'une redirection de sortie TRUNC (>)\n");
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
    else if (type == ERR_REDIR)
        return ft_strdup("2>");
    return (NULL);
}

int ft_redirection(t_com_list *command, int *mem_fd_in, int *mem_fd_out, int *mem_fd_err)
{
    int fd;
    t_file_list *tmp;

    // Gestion de l'entrée
    if (command->infile)
    {
        printf("Redirection d'entrée: %s\n", command->infile);
        fd = open_file_cmd(command->infile);
        if (fd < 0)
        {
            perror(command->infile);
            return (-1);
        }
        if (mem_fd_in)
            *mem_fd_in = dup(STDIN_FILENO);
        dup2(fd, STDIN_FILENO);
        close(fd);
    }

    // Gestion de la sortie principale
    if (command->outfile)
    {
        printf("Redirection de sortie: %s\n", command->outfile);
        if (mem_fd_out)
            *mem_fd_out = dup(STDOUT_FILENO);
        fd = open_outfile(command->outfile, command->flag_out);
        if (fd < 0)
        {
            perror(command->outfile);
            return (-1); // Ne pas créer quoi que ce soit d’autre
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }

    // Gestion de tous les outfiles secondaires
    tmp = command->all_outfilles;
    while (tmp)
    {
        printf("Redirection de sortie secondaire: %s\n", tmp->filename);
        fd = open_outfile(tmp->filename, tmp->flag);
        if (fd < 0)
        {
            perror(tmp->filename);
            return (-1);
        }
        close(fd); // just to create it
        tmp = tmp->next;
    }

    // Gestion de l'erreur standard
    if (command->errfile)
    {
        if (mem_fd_err)
            *mem_fd_err = dup(STDERR_FILENO);
        fd = open_errfile(command->errfile);
        if (fd < 0)
        {
            perror(command->errfile);
            return (-1);
        }
        dup2(fd, STDERR_FILENO);
        close(fd);
    }

    return (0);
}

void restore_redirections(int mem_fd_in, int mem_fd_out, int mem_fd_err)
{
    if (mem_fd_in != -1)
    {
        dup2(mem_fd_in, STDIN_FILENO);
        close(mem_fd_in);
    }
    if (mem_fd_out != -1)
    {
        dup2(mem_fd_out, STDOUT_FILENO);
        close(mem_fd_out);
    }
    if (mem_fd_err != -1)
    {
        dup2(mem_fd_err, STDERR_FILENO);
        close(mem_fd_err);
    }
}
