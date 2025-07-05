# Minishell
Shell Unix simplifié avec pipes, redirections et builtins - Projet 42

![Minishell](https://img.shields.io/badge/Shell-Minishell-red)
![42](https://img.shields.io/badge/42-Project-brightgreen)

## 📝 Description

Minishell est une implémentation simplifiée d'un shell Unix similaire à bash. Ce projet fait partie du cursus de l'école 42 et se concentre sur la création de processus, l'exécution de commandes, la gestion des signaux et l'analyse des commandes avec différents opérateurs.

## 🚀 Fonctionnalités

- Exécution de commandes avec arguments
- Historique des commandes
- Gestion des variables d'environnement
- Pipes (`|`)
- Redirections (`>`, `<`, `>>`, `<<`)
- Gestion des signaux (Ctrl+C, Ctrl+D, Ctrl+\)
- Commandes intégrées (builtins) :
  - `echo` avec l'option `-n`
  - `cd` avec chemin relatif ou absolu
  - `pwd` sans options
  - `export` sans options
  - `unset` sans options
  - `env` sans options ni arguments
  - `exit` sans options
  - `history` sans options

## 🛠️ Installation

### Prérequis

- Compilateur CC
- Make
- Bibliothèque Readline

### Cloner le dépôt

```bash
git clone https://github.com/ambroiselebs/42-minishell.git minishell
cd minishell
```

### Compiler

```bash
make
```

## 🔍 Utilisation

### Lancer le shell

```bash
./minishell
```

### Exemples

```bash
$> ls -la
# Liste les fichiers du répertoire courant avec détails

$> echo "Bonjour, Monde !"
Bonjour, Monde !

$> cat fichier.txt | grep "motif" > resultat.txt
# Pipes et redirections

$> export MA_VAR=42
# Définit une variable d'environnement

$> echo $MA_VAR
42
# Affiche la valeur de la variable d'environnement

$> cd /chemin/vers/repertoire
# Change de répertoire courant

$> pwd
/chemin/vers/repertoire
# Affiche le répertoire courant

$> exit
# Quitte le shell
```

## ⚙️ Détails d'implémentation

### Architecture

Le projet est organisé en plusieurs modules :

1. **Lexer/Tokenizer** : Découpe la ligne de commande en jetons (tokens)
2. **Parser** : Interprète les jetons en structures de commandes
3. **Expander** : Gère l'expansion des variables d'environnement
4. **Executor** : Exécute les commandes et gère les pipes/redirections
5. **Builtins** : Implémente les commandes intégrées du shell
6. **Gestionnaire de signaux** : Gère les interruptions par signal
7. **Gestionnaire d'environnement** : Gère les variables d'environnement

### Fichiers clés

- `main.c` : Point d'entrée du programme
- `minishell.h` : Fichier d'en-tête principal avec toutes les structures et prototypes
- `src/token/` : Fonctions liées à la tokenisation
- `src/parsing/` : Fonctions d'analyse des commandes
- `src/expand/` : Fonctions d'expansion des variables
- `src/exec/` : Fonctions d'exécution des commandes
- `src/builtin/` : Implémentation des commandes intégrées
- `src/signals/` : Gestion des signaux
- `libs/` : Bibliothèques de support

## 📋 Exigences du projet

- Doit gérer l'historique des commandes
- Doit chercher et lancer les exécutables basés sur la variable PATH ou un chemin relatif/absolu
- Doit gérer les guillemets (`'`, `"`) pour désactiver l'interprétation des caractères spéciaux
- Doit implémenter les redirections (`<`, `>`, `<<`, `>>`)
- Doit implémenter les pipes (`|`)
- Doit gérer l'expansion des variables d'environnement (`$VAR`)
- Doit gérer les variables spéciales du shell (`$?`)
- Doit gérer les signaux (Ctrl+C, Ctrl+D, Ctrl+\)
- Doit implémenter les commandes intégrées suivantes :
  - `echo` avec l'option `-n`
  - `cd` avec chemin relatif ou absolu
  - `pwd` sans options
  - `export` sans options
  - `unset` sans options
  - `env` sans options ni arguments
  - `exit` sans options

## 👥 Auteurs

- BERENGER Ambroise
- PAPIN Marius
