/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 01:20:51 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/25 01:36:39 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Crée une nouvelle commande vide.
** Retourne un pointeur vers la commande créée, NULL en cas d'erreur.
*/
t_cmd	*create_command(void)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->path = NULL;
	cmd->args = NULL;
	cmd->redir = NULL;
	cmd->tokens = NULL;
	cmd->next = NULL;
	return (cmd);
}

/*
** Ajoute une commande à la fin de la liste de commandes.
** Si la liste est vide, initialise la liste avec la commande.
*/
void	add_command(t_cmd **cmd_list, t_cmd *cmd)
{
	t_cmd	*current;

	if (!cmd_list || !cmd)
		return ;
	if (*cmd_list == NULL)
	{
		*cmd_list = cmd;
		return ;
	}
	current = *cmd_list;
	while (current->next)
		current = current->next;
	current->next = cmd;
}

/*
** Libère la mémoire d'une liste de commandes.
** Retourne toujours NULL pour faciliter la gestion d'erreur.
*/
void	*free_commands(t_cmd *cmd_list)
{
	t_cmd	*current;
	t_cmd	*next;

	current = cmd_list;
	while (current)
	{
		next = current->next;
		free_command(current);
		current = next;
	}
	return (NULL);
}

/*
** Libère la mémoire d'une commande.
*/
void	free_command(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->path)
		free(cmd->path);
	if (cmd->args)
		ft_free_tab(cmd->args);
	if (cmd->tokens)
		free_tokens(cmd->tokens);
	if (cmd->redir)
		free_redirections(cmd->redir);
	free(cmd);
}

/*
** Libère la mémoire d'une liste de redirections.
*/
void	free_redirections(t_redir *redir)
{
	t_redir	*current;
	t_redir	*next;

	current = redir;
	while (current)
	{
		next = current->next;
		if (current->file)
			free(current->file);
		free(current);
		current = next;
	}
}
