/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 01:40:51 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/28 18:37:23 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**fill_args(t_token *tokens, int count)
{
	char	**args;
	int		i;
	t_token	*current;

	args = (char **) malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	i = 0;
	current = tokens;
	while (current && i < count)
	{
		if (current->type == WORD)
		{
			if (!allocate_arg(args, i, current->value))
				return (NULL);
			i++;
		}
		current = current->next;
	}
	args[i] = NULL;
	return (args);
}

/*
** Prépare les arguments pour une commande à partir des tokens.
** Retourne 1 en cas de succès, 0 en cas d'erreur.
** Une commande sans tokens mais avec des redirections est valide.
*/
int	prepare_args(t_cmd *cmd)
{
	int	count;

	if (!cmd)
		return (0);
	if (!cmd->tokens && cmd->redir)
		return (1);
	if (!cmd->tokens)
		return (0);
	count = count_args(cmd->tokens);
	if (count == 0)
		return (1);
	cmd->args = fill_args(cmd->tokens, count);
	if (!cmd->args)
		return (0);
	cmd->path = ft_strdup(cmd->args[0]);
	if (!cmd->path)
	{
		ft_free_tab(cmd->args);
		cmd->args = NULL;
		return (0);
	}
	return (1);
}

/*
** Prépare les arguments pour toutes les commandes d'une liste.
** Retourne 1 en cas de succès, 0 en cas d'erreur.
** Prend en compte les commandes qui n'ont que des redirections.
*/
int	prepare_all_args(t_cmd *cmd_list)
{
	t_cmd	*current;

	current = cmd_list;
	while (current)
	{
		if (current->redir && !current->tokens)
			current = current->next;
		else if (!prepare_args(current))
			return (0);
		else
			current = current->next;
	}
	return (1);
}

/*
** Remplace les chemins relatifs des commandes par leurs chemins absolus.
** Utilise la variable d'environnement PATH pour chercher les exécutables.
** Retourne 1 en cas de succès, 0 en cas d'erreur.
** Prend en compte les commandes qui n'ont que des redirections.
*/
static void	update_path(t_cmd *cmd, char *path_env)
{
	char	*exec_path;

	if (cmd->path && cmd->path[0] != '/' && ft_strncmp(cmd->path, "./", 2) != 0)
	{
		exec_path = find_executable(cmd->path, path_env);
		if (exec_path)
		{
			free(cmd->path);
			cmd->path = exec_path;
		}
	}
}

int	resolve_paths(t_cmd *cmd_list, t_env *env)
{
	t_cmd	*current;
	char	*path_env;

	current = cmd_list;
	path_env = get_env_value(env, "PATH");
	while (current)
	{
		if (!current->path && current->redir)
		{
			current = current->next;
			continue ;
		}
		update_path(current, path_env);
		current = current->next;
	}
	return (1);
}
