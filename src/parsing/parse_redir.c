/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 01:30:51 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/25 01:40:02 by aberenge         ###   ########.fr       */
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
** Gère les signaux pendant l'exécution du heredoc
*/
static void	handle_heredoc_signals(int signum)
{
	(void)signum;
	write(STDERR_FILENO, "\n", 1);
	g_heredoc_interrupted = 1;
	exit(130);  // 128 + SIGINT
}

/*
** Configure les gestionnaires de signaux pour le mode heredoc
*/
static void	setup_heredoc_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_heredoc_signals;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	
	// Ignorer SIGQUIT
	signal(SIGQUIT, SIG_IGN);
}

/*
** Restaure les gestionnaires de signaux par défaut
** Note: Cette fonction devrait être appelée dans le parent
** après l'exécution du heredoc si nécessaire.
*/
static void	reset_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_DFL;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
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
	
	g_heredoc_interrupted = 0;
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
		// Process enfant pour lire l'entrée
		setup_heredoc_signals();
		close(pipe_fd[0]);  // Fermer l'extrémité de lecture dans l'enfant
		
		while (1)
		{
			// Lire depuis l'entrée standard
			line = readline("> ");
			if (!line || ft_strcmp(line, delimiter) == 0)
			{
				if (line)
					free(line);
				break;
			}
			
			// Écrire dans le pipe
			ft_putstr_fd(line, pipe_fd[1]);
			ft_putstr_fd("\n", pipe_fd[1]);
			free(line);
		}
		
		close(pipe_fd[1]);
		exit(0);
	}
	
	// Processus parent
	close(pipe_fd[1]);  // Fermer l'extrémité d'écriture dans le parent
	
	// Attendre la fin du processus enfant
	waitpid(pid, &status, 0);
	
	// Restaurer les signaux par défaut
	reset_signals();
	
	// Vérifier si l'enfant a été interrompu par un signal
	if (WIFSIGNALED(status))
	{
		g_heredoc_interrupted = 1;
		close(pipe_fd[0]);
		return (-1);
	}
	
	// Vérifier le code de sortie
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		g_heredoc_interrupted = 1;
		close(pipe_fd[0]);
		return (-1);
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
			if (g_heredoc_interrupted)
			{
				g_return_code = 130;
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
	// Si le fichier est NULL, c'est probablement dû à un heredoc interrompu
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
