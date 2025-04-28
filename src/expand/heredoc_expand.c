/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 18:45:51 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/28 18:45:51 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Crée un nom de fichier temporaire pour le heredoc
** dans le répertoire home de l'utilisateur.
*/
static void	get_temp_heredoc_file(char *dest)
{
	char	*home;

	home = getenv("HOME");
	if (!home)
		home = "/tmp";
	ft_strcpy(dest, home);
	ft_strcat(dest, "/.minishell_heredoc");
}

/*
** Lit et stocke l'entrée du heredoc dans un fichier temporaire
** jusqu'à ce que le délimiteur soit trouvé.
*/
static void	handle_heredoc_child(const char *filename, const char *delimiter)
{
	char	*line;
	int		fd;

	setup_signals_heredoc();
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd == -1)
		exit(1);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			if (line)
				free(line);
			break ;
		}
		ft_putstr_fd(line, fd);
		ft_putstr_fd("\n", fd);
		free(line);
	}
	close(fd);
	rl_clear_history();
	// Cleanup pour éviter les still reachable
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	exit(0);
}

/*
** Gère le processus parent du heredoc
*/
static int	handle_heredoc_parent(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	setup_signals_interactive();
	if (WIFSIGNALED(status) || (WIFEXITED(status) && WEXITSTATUS(status) == 130))
	{
		g_return_code = 130;
		return (0);
	}
	return (1);
}

/*
** Recherche tous les tokens de type HEREDOC et les remplace par des REDIR_IN
** qui pointent vers un fichier temporaire contenant l'entrée du heredoc
*/
void	expand_heredocs(t_token *tokens)
{
	t_token	*current;
	t_token	*next;
	char	filename[PATH_MAX];
	char	*file_dup;
	pid_t	pid;

	current = tokens;
	while (current && current->next)
	{
		if (current->type == HEREDOC)
		{
			next = current->next;
			get_temp_heredoc_file(filename);
			pid = fork();
			if (pid == -1)
				return ;
			if (pid == 0)
				handle_heredoc_child(filename, next->value);
			if (!handle_heredoc_parent(pid))
				return ;
			file_dup = ft_strdup(filename);
			if (!file_dup)
				return ;
			free(next->value);
			next->value = file_dup;
			current->type = REDIR_IN;
		}
		current = current->next;
	}
}