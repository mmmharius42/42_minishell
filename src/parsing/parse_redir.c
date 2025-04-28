/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 01:30:51 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/28 18:36:35 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Traite une redirection en vérifiant que le prochain token
** est bien un nom de fichier.
** Retourne 1 en cas de succès, 0 en cas d'erreur.
**
** Attention: on passe token->next pour avancer le pointeur correctement
** dans process_token_to_cmd().
*/
int	process_redirection(t_token *token, t_cmd **cmd)
{
	t_token	*filename_token;
	t_redir	*redir;

	if (!token || !token->next || token->next->type != WORD)
		return (parse_error("syntax error near redirection"));
	filename_token = token->next;
	redir = create_redirection(token->type, filename_token->value);
	if (!redir)
		return (0);
	add_redirection(&((*cmd)->redir), redir);
	return (1);
}

/*
** Crée une nouvelle redirection.
** Retourne un pointeur vers la redirection créée, NULL en cas d'erreur.
*/
t_redir	*create_redirection(int type, char *file)
{
	t_redir	*redir;

	redir = (t_redir *)malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = ft_strdup(file);
	if (!redir->file)
	{
		free(redir);
		return (NULL);
	}
	redir->next = NULL;
	return (redir);
}

/*
** Applique les redirections pour une commande.
** Retourne 1 en cas de succès, 0 en cas d'erreur.
*/
static int	open_redir_file(t_redir *redir, int *fd)
{
	if (redir->type == REDIR_IN)
		*fd = open(redir->file, O_RDONLY);
	else if (redir->type == REDIR_OUT)
		*fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == APPEND)
		*fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	return (1);
}

static int	apply_redir_fd(t_redir *redir, int fd)
{
	if (fd == -1)
	{
		g_return_code = 1;
		return (redirect_error(redir->file));
	}
	if (redir->type == REDIR_IN || redir->type == HEREDOC)
		dup2(fd, STDIN_FILENO);
	else
		dup2(fd, STDOUT_FILENO);
	close(fd);
	return (1);
}

int	apply_redirections(t_redir *redir)
{
	t_redir	*current;
	int		fd;

	current = redir;
	while (current)
	{
		if (!open_redir_file(current, &fd))
			return (0);
		if (current->type == REDIR_IN || current->type == REDIR_OUT
			|| current->type == APPEND || current->type == HEREDOC)
		{
			if (!apply_redir_fd(current, fd))
				return (0);
		}
		current = current->next;
	}
	return (1);
}
