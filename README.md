# Minishell
Projet realise dans le cadre du Common Core a 42 Paris par juvitry and opique.

> **Minishell** est une réplique simplifiée d’un terminal shell (inspirée de Bash), créée dans le cadre du cursus **42 Paris**.  
> L’objectif est d’implémenter et comprendre les mécanismes de base d’un *shell* : lecture de ligne, parsing, exécution de commandes, redirections, pipes, gestion des signaux et variables d’environnement.

---


## 📌 Table des matières

- [🧠 Objectif](#🧠-objectif)  
- [🚀 Fonctionnalités](#🚀-fonctionnalités)  
- [🛠️ Installation & Utilisation](#🛠️-installation--utilisation)  
- [🧩 Architecture du projet](#🧩-architecture-du-projet)  
- [📌 Limitations & Comportement attendu](#📌-limitations--comportement-attendu)  
- [📊 Évaluation & Résultats](#📊-évaluation--résultats)  
- [✨ À améliorer](#✨-à-ameliorer)  
- [📄 Licence](#📄-licence)

---

## 🧠 Objectif

Ce projet a pour but de :

- Comprendre comment fonctionne un *shell* Unix.
- Implémenter les éléments essentiels tels que : lecture interactive, parsing, exécution de commandes, piping, redirection, gestion d’environnements.
- Passer l’évaluation du sujet **Minishell** du cursus 42.

Ce n’est **pas** un clone complet de Bash mais une version **fonctionnelle et éducative**.

---

## 🚀 Fonctionnalités

Minishell supporte :

### 📌 Commandes

- Exécution de commandes externes (`ls`, `grep`, `cat`, etc.)
- Chemins relatifs ou absolus

### 📌 Builtin

- `echo`
- `cd`
- `pwd`
- `export`
- `unset`
- `env`
- `exit`

### 📌 Redirections

- `>` : redirection de sortie  
- `>>` : ajout en sortie  
- `<` : redirection d’entrée  
- `<< DELIM` : *here-document*

### 📌 Pipes

- `|` : relie la sortie d’une commande à l’entrée d’une autre

### 📌 Signaux

- `Ctrl-C` : nouvelle ligne de prompt  
- `Ctrl-D` : quitte le shell  
- `Ctrl-\` : ignoré au prompt

---

## 🛠️ Installation & Utilisation

### 📦 Cloner le dépôt

```bash
git clone https://github.com/Julienv28/Minishell.git
cd Minishell
```

### 🛠️ Compilation
```bash
make
```
### ▶️ Exécution
```bash
./minishell
```
---
## 🧩 Architecture du projet

`includes/` : headers

`srcs/`: code source

`libft/` : libft (bibliothèque utilitaire)

`Makefile` : compilation

Chaque phase du shell est gérée par une étape claire :
1. Lecture interactive (readline)
2. Parsing lexical et syntaxique
3. Expansion des variables & traitement des quotes
4. Exécution des commandes

---

## 📌 Limitations & Comportement attendu

Dans sa forme actuelle, Minishell ne supporte pas :
- Opérateurs logiques (`&&`, `||`)
- Points-virgules (`;`)
- Wildcards (`*`)

Substitutions complexes avancées
---

## 📊 Évaluation & Résultats

Ce projet a été réalisé dans le cadre du cursus 42 Paris :
✔️ Objectifs fonctionnels atteints ✔️

📌 Quelques fuites _still reachable_ ont été observées avec Valgrind et sont à corriger pour une version plus robuste.

### ✨ À améliorer

Suggestions d’amélioration :
- Ajouter des tests unitaires / CI (GitHub Actions);
- Support des opérateurs (`&&`, `||`);
- Gestion avancée des erreurs et des retours d’état;
- Support des jokers (`*`) et globbing.

---

## 📄 Licence

Ce projet est sous licence GPL-3.0.

---

Some advices for surviving the examination :
- Stay aware that the readline package and external commands may cause leaks ad still reachables. In tht case, think about adding a .supp file that clean these errors on Valgrind so that you can get away easily.
- Make sure that unsetting environment variables as $PATH doesnot cause any leak. We forgot it for our first try causing us to retry it.
- Make sure to use the global variable *only* for Signals. Exit status must be sorted as a separate variable that shouldn't be a global one (the best thing is to but it within a structure).
- Don't hesitate to check this [excel_file](https://docs.google.com/spreadsheets/d/1uJHQu0VPsjjBkR4hxOeCMEt3AOM1Hp_SmUzPFhAH-nA/edit?gid=0#gid=0) containing 800+ tests, if you pass 75% of them, then you should be able to validate and have a project that is solid enough. There are also several cas figures that should help(some you never think about but that may be useful).
- Run this to check every leaks valgrind --leak-check=full --trace-children=yes --track-fds=yes --suppressions=readline.supp (optionnal if you have a .supp file) -q (only shows when there are big leaks) ./minishell

Good Luck !

__Splitting of work :__
- Parsing, signals, debug and error handling : Oceane
- Execution, Pipes, environment varaiables and leaks : Julien

The implementation of the different functions (cd, env, export, unset, echo and pwd) was splitted among the two of us. Even Though the splitting was quite fixed, we improved each other's code when it was necessary so we both know how the entire code works.
