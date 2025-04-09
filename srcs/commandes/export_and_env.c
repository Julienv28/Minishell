#include "../includes/minishell.h"

// modifier variable d'enironnement setenv() et unsetenv()

// EXPORT
// Un environnement est un ensemble de variables qui=e l'on peut modifer pour personnalise l'espace de travail
// Permet d'afficher l'environnement, de le creer mais aussi de l'exporter dans un autre fichierfichier 
// Permet d'ajouter ou de modifier une variable d'environnement et de la rendre disponible dans tous les processus fils du shell (zsh, sh ..)

// Afficher l'environnement (export)
// Creer une variable aui sera traitee dans l'environnement (export MA_VARIABLE)


// export VAR=value : pour definir une variable d'environnement
// export : pour afficher toutes les variables exportees

// 1. Savoir si la commande contient un nom de variable avec une valeur assigner
// 2. Si c'est le cas, il faut verifier la validite du nom de la variable (contenir que des lettres, chiffres et _ )
// 3. L'ajouter au tableau des variables d'environnement
// 4. Si elle existe deja la mettre a jour
// 5. La rendre accessible aux sous processus. Utilisation setenv() pour modifier l'environnement processus
// 6. Si utilisaeur tape juste EXPORT sans arg, afficher toutes les variables d'environnement exportes.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Fonction pour vérifier la validité du nom de la variable
int is_valid_name(char *name)
{
    int i;

    if (!name || (!ft_isalpha(name[0]) && name[0] != '_')) // Le nom doit commencer par une lettre ou un underscore
        return (0);
    i = 1;
    while (name[i])
    {
        if (!(ft_isalnum(name[i]) || name[i] == '_')) // Le reste doit être alphanumérique ou underscore
        {
            printf("restant lettres fausses\n");
            return (0);
        }
        i++;
    }
    return (1);
}

// Fonction pour effectuer l'exportation d'une variable d'environnement
void ft_export(char **args, char **envp)
{
    int i;
    char *var_env;   // variable d'environnement
    char *value;     // Valeur de la variable
    char *name;      // Nom de la variable
    char *egal_sign; // Chercher signe = pour définir nom et valeur
    char **new_envp;
    int env_count = 0;

    i = 0;
    if (args[1] == NULL) // Si juste EXPORT afficher
    {
        while (envp[i])
        {
            printf("%s\n", envp[i]); // Afficher chaque variable d'environnement
            i++;
        }
    }
    else
    {
        var_env = args[1];
        printf("variable env = %s\n", var_env);
        egal_sign = ft_strchr(var_env, '='); // Chercher signe '=' pour définir la valeur et le nom
        if (egal_sign == NULL)
        {
            ft_putstr_fd("Erreur: Variable d'environnement invalide (pas de '=')", STDERR_FILENO);
            return;
        }
        *egal_sign = '\0'; // Remplacer '=' par '\0' pour séparer le nom et la valeur
        name = var_env; // Le nom de la variable
        value = egal_sign + 1; // La valeur après '='
        
        printf("name = %s\n", name);
        printf("value = %s\n", value);
        // Vérifier que le nom de la variable est valide
        if (!is_valid_name(name))
        {
            ft_putstr_fd("Erreur: nom_variable", STDERR_FILENO);
            return;
        }

        // Ajouter ou modifier la variable d'environnement
        if (setenv(name, value, 1) == 0)
            printf("variable %s=%s exportée avec succès\n", name, value);
        else
            perror("Erreur lors de l'exportation");
        
        while (envp[env_count])  // Calculer la taille de envp
            env_count++;

        // Réallouer de l'espace pour le nouveau tableau avec la nouvelle variable
        new_envp = malloc(sizeof(char *) * (env_count + 2)); // +2 pour la nouvelle variable et le NULL final
        if (new_envp == NULL)
            return;
        
        // Copier les anciennes variables dans le nouveau tableau
        i = 0;
        while (i < env_count)
        {
            new_envp[i] = ft_strdup(envp[i]); // Copier la chaîne entière
            i++;
        }

        // Ajouter la nouvelle variable
        new_envp[env_count] = ft_strdup(args[1]); // Copier la nouvelle variable

        // Assurez-vous que le tableau est terminé par NULL
        new_envp[env_count + 1] = NULL;

        // Libérer l'ancien tableau et mettre à jour envp
        free(*envp);  // Libérer l'ancien tableau
        *envp = new_envp;  // Mettre à jour *envp pour pointer vers le nouveau tableau
    }
}

// ENV
// Affiche mon environnement et permet de manipuler l'environnemet pour lancer une commande.
// Peut modifier temporairement l'environnement pour executer une comande puis l'environnement reviens a la norme

// 1. Si l'utilisateur tape ENV afficher les variables atuelles
// 2. Si elle est suivie d'une variable modifie et d'une cmd :
// 3. Creer un environnement temporaire avec les nouvelles variables specifiees dans la cmd modifees
// 4. Lancer la cmd specifiee avec cet environnement temporaire. Donc cree un new processus (fork() et exec())
// 5. Pour exécuter une commande avec un environnement modifié, utiliser setenv() pour modifer l'environnement 