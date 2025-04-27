/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 01:30:51 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/27 21:01:03 by aberenge         ###   ########.fr       */
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
** Ajoute une redirection à la fin de la liste de redirections.
** Si la liste est vide, initialise la liste avec la redirection.
*/
void	add_redirection(t_redir **redir_list, t_redir *redir)
{
	t_redir	*current;

	if (!redir_list || !redir)
		return ;
	if (*redir_list == NULL)
	{
		*redir_list = redir;
		return ;
	}
	current = *redir_list;
	while (current->next)
		current = current->next;
	current->next = redir;
}

/*
** Lit et stocke l'entrée du heredoc jusqu'à ce que
** le délimiteur soit trouvé.
** Retourne un descripteur de fichier pour lire le contenu,
** ou -1 en cas d'erreur.
*/
static int	handle_heredoc(char *delimiter)
{
	char	*line;
	int		pipe_fd[2];
	pid_t	pid;
	int		status;

	if (pipe(pipe_fd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (-1);
	}
	if (pid == 0)
	{
		setup_signals_heredoc();
		close(pipe_fd[0]);
		while (1)
		{
			line = readline("> ");
			if (!line || ft_strcmp(line, delimiter) == 0)
			{
				if (line)
					free(line);
				break ;
			}
			ft_putstr_fd(line, pipe_fd[1]);
			ft_putstr_fd("\n", pipe_fd[1]);
			free(line);
		}
		close(pipe_fd[1]);
		exit(0);
	}
	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	setup_signals_interactive();
	if (WIFSIGNALED(status) || (WIFEXITED(status) && WEXITSTATUS(status) == 130))
	{
		g_return_code = 130;
		close(pipe_fd[0]);
		return (-130);
	}
	return (pipe_fd[0]);
}

/*
** Applique les redirections pour une commande.
** Retourne 1 en cas de succès, 0 en cas d'erreur.
*/
int	apply_redirections(t_redir *redir)
{
	t_redir	*current;
	int		fd;

	current = redir;
	while (current)
	{
		if (current->type == REDIR_IN)
			fd = open(current->file, O_RDONLY);
		else if (current->type == REDIR_OUT)
			fd = open(current->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (current->type == APPEND)
			fd = open(current->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else if (current->type == HEREDOC)
		{
			fd = handle_heredoc(current->file);
			if (fd == -130)
			{
				ft_putstr_fd("minishell: heredoc interrompu\n", 2);
				return (0);
			}
		}
		else
			continue ;
		if (fd == -1)
		{
			g_return_code = 1;
			return (redirect_error(current->file));
		}
		if (current->type == REDIR_IN || current->type == HEREDOC)
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);
		close(fd);
		current = current->next;
	}
	return (1);
}

/*
** Affiche une erreur de redirection.
** Retourne toujours 0 pour indiquer une erreur.
*/
int	redirect_error(char *file)
{
	if (!file)
	{
		ft_putstr_fd("minishell: heredoc interrompu\n", 2);
		return (0);
	}
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(file, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
	g_return_code = 1;
	return (0);
}
