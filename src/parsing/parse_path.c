/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 01:50:51 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/25 02:45:00 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Vérifie si un fichier existe et est exécutable.
** Retourne 1 si le fichier existe et est exécutable, 0 sinon.
*/
int	is_executable(char *path)
{
	struct stat	st;

	if (stat(path, &st) == -1)
		return (0);
	if (S_ISDIR(st.st_mode))
		return (0);
	if (!(st.st_mode & S_IXUSR))
		return (0);
	return (1);
}

/*
** Crée un chemin complet à partir d'un chemin de base et d'un nom de fichier.
** Retourne le chemin complet, NULL en cas d'erreur.
*/
char	*create_path(char *dir, char *file)
{
	char	*path;
	char	*tmp;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	path = ft_strjoin(tmp, file);
	free(tmp);
	return (path);
}

/*
** Cherche un exécutable dans les chemins spécifiés par la variable PATH.
** Retourne le chemin complet de l'exécutable, NULL s'il n'est pas trouvé.
*/
char	*find_executable(char *cmd, char *path_env)
{
	char	**paths;
	char	*path;
	int		i;

	if (!cmd || !path_env)
		return (NULL);
	paths = ft_split(path_env, ":");
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		path = create_path(paths[i], cmd);
		if (!path)
			break ;
		if (is_executable(path))
		{
			ft_free_tab(paths);
			return (path);
		}
		free(path);
		i++;
	}
	ft_free_tab(paths);
	return (NULL);
}

/*
** Vérifie si une commande est un builtin.
** Retourne 1 si c'est un builtin, 0 sinon.
*/
int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

/*
** Finalise la préparation des commandes.
** Vérifie si chaque commande est un builtin ou cherche son chemin.
** Retourne 1 en cas de succès, 0 en cas d'erreur.
*/
int	finalize_commands(t_cmd *cmd_list, t_env *env)
{
	if (!prepare_all_args(cmd_list))
		return (0);
	if (!resolve_paths(cmd_list, env))
		return (0);
	return (1);
}
