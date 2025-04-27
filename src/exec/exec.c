/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 17:47:09 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/25 19:48:33 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_return_code = 0;
int	g_heredoc_interrupted = 0;

static char	**env_to_array(t_env *env)
{
	t_env	*current;
	char	*tmp;
	char	**env_array;
	int		i;
	int		size;

	size = 0;
	current = env;
	while (current)
	{
		if (current->equal_sign)
			size++;
		current = current->next;
	}
	env_array = malloc(sizeof(char *) * (size + 1));
	if (!env_array)
		return (NULL);
	current = env;
	i = 0;
	while (current)
	{
		if (current->equal_sign)
		{
			tmp = ft_strjoin(current->name, "=");
			env_array[i] = ft_strjoin(tmp, current->value);
			free(tmp);
			i++;
		}
		current = current->next;
	}
	env_array[i] = NULL;
	return (env_array);
}

static void	execute_command(t_cmd *cmd, t_env *env)
{
	char	**env_array;

	// Configuration des signaux pour les processus enfant
	setup_signals_child();

	// Appliquer les redirections (entrée/sortie) avant l'exécution de la commande
	if (cmd->redir && !apply_redirections(cmd->redir))
		exit(g_return_code);
	
	env_array = env_to_array(env);
	if (!env_array)
		exit(1);
	if (execve(cmd->path, cmd->args, env_array) == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
}

static void	execute_builtin_with_redir(t_cmd *cmd, t_env **env)
{
	int		stdin_backup;
	int		stdout_backup;
	
	// Sauvegarder les descripteurs de fichiers standards
	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);
	
	// Appliquer les redirections si elles existent
	if (cmd->redir && !apply_redirections(cmd->redir))
	{
		g_return_code = 1;
		return;
	}
	
	// Exécuter la commande builtin
	exec_builtin(cmd, (char ***)env);
	
	// Restaurer les descripteurs standards
	dup2(stdin_backup, STDIN_FILENO);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdin_backup);
	close(stdout_backup);
}

static void	execute_simple_command(t_cmd *cmd, t_env **env)
{
	pid_t	pid;
	int		status;

	// Commande builtin sans pipe: exécuter directement sans fork
	if (check_builtin(cmd))
	{
		execute_builtin_with_redir(cmd, env);
		return;
	}
	
	// Commande externe: fork nécessaire
	pid = fork();
	if (pid == -1)
	{
		g_return_code = 1;
		return;
	}
	if (pid == 0)
	{
		execute_command(cmd, *env);
		exit(g_return_code);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			g_return_code = WEXITSTATUS(status);
	}
}

static void	execute_piped_commands(t_cmd *cmd_list, t_env *env)
{
	pid_t	pid;
	int		status;
	int		pipe_fd[2];
	int		prev_pipe;
	t_cmd	*current;

	current = cmd_list;
	prev_pipe = -1;
	while (current)
	{
		if (current->next)
			pipe(pipe_fd);
		pid = fork();
		if (pid == 0)
		{
			if (prev_pipe != -1)
			{
				dup2(prev_pipe, STDIN_FILENO);
				close(prev_pipe);
			}
			if (current->next)
			{
				close(pipe_fd[0]);
				dup2(pipe_fd[1], STDOUT_FILENO);
				close(pipe_fd[1]);
			}
			
			// Appliquer les redirections spécifiques à cette commande
			if (current->redir && !apply_redirections(current->redir))
				exit(g_return_code);
				
			// Exécuter la commande builtin ou externe
			if (check_builtin(current))
				exec_builtin(current, (char ***)&env);
			else
				execute_command(current, env);
			exit(g_return_code);
		}
		if (prev_pipe != -1)
			close(prev_pipe);
		if (current->next)
		{
			close(pipe_fd[1]);
			prev_pipe = pipe_fd[0];
		}
		current = current->next;
	}
	current = cmd_list;
	while (current)
	{
		waitpid(-1, &status, 0);
		if (WIFEXITED(status))
			g_return_code = WEXITSTATUS(status);
		current = current->next;
	}
}

void	exec(t_cmd *cmd, t_env **env)
{
	if (!cmd)
		return;
	
	// Traiter les redirections même s'il n'y a pas de commande à exécuter
	if (!cmd->path && cmd->redir)
	{
		pid_t pid;
		int status;
		
		setup_signals_parent(); // Ignorer les signaux dans le parent
		
		pid = fork();
		if (pid == 0)
		{
			setup_signals_child(); // Définir les gestionnaires pour l'enfant
			
			// Appliquer les redirections
			if (!apply_redirections(cmd->redir))
				exit(g_return_code);
			exit(0);
		}
		else
		{
			waitpid(pid, &status, 0);
			setup_signals_interactive(); // Restaurer les gestionnaires interactifs
			
			if (WIFEXITED(status))
				g_return_code = WEXITSTATUS(status);
			return;
		}
	}
	
	// Exécuter la commande si elle existe
	if (!cmd->path)
		return;
	
	// Configurer les signaux avant l'exécution
	setup_signals_parent(); // Ignorer les signaux dans le processus parent
	
	if (!cmd->next)
		execute_simple_command(cmd, env);
	else
		execute_piped_commands(cmd, *env);
		
	// Réinitialiser les signaux après l'exécution
	setup_signals_interactive();
}