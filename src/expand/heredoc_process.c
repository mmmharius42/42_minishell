/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_process.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 19:15:51 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/28 18:45:55 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Nettoie les ressources du processus heredoc
*/
void	clean_heredoc(t_token *tokens, t_env *env)
{
	rl_clear_history();
	free_env(env);
	free_tokens(tokens);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}

/*
** Crée un nom de fichier temporaire pour le heredoc
** dans le répertoire home de l'utilisateur.
*/
void	get_temp_heredoc_file(char *dest)
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
void	handle_heredoc_child(const char *filename, const char *delimiter,
							t_env *env, t_token *tokens)
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
	clean_heredoc(tokens, env);
	exit(0);
}

/*
** Gère le processus parent du heredoc
*/
int	handle_heredoc_parent(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	setup_signals_interactive();
	if (WIFSIGNALED(status) || (WIFEXITED(status)
			&& WEXITSTATUS(status) == 130))
	{
		g_return_code = 130;
		return (0);
	}
	return (1);
}

/*
** Traite un token HEREDOC en créant un fichier temporaire
*/
int	process_heredoc_token(t_token *token, t_token *next, t_env *env,
					t_token *tokens)
{
	char	filename[PATH_MAX];
	char	*file_dup;
	pid_t	pid;

	get_temp_heredoc_file(filename);
	pid = fork();
	if (pid == -1)
		return (0);
	if (pid == 0)
		handle_heredoc_child(filename, next->value, env, tokens);
	if (!handle_heredoc_parent(pid))
		return (0);
	file_dup = ft_strdup(filename);
	if (!file_dup)
		return (0);
	free(next->value);
	next->value = file_dup;
	token->type = REDIR_IN;
	return (1);
}
