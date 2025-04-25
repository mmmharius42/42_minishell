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

static void	execute_simple_command(t_cmd *cmd, t_env **env)
{
	pid_t	pid;
	int		status;

	// Commande builtin sans pipe: exécuter directement sans fork
	if (check_builtin(cmd))
	{
		exec_builtin(cmd, (char ***)env);
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
		exit(0);
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
			// Exécuter la commande builtin ou externe
			if (check_builtin(current))
				exec_builtin(current, (char ***)&env);
			else
				execute_command(current, env);
			exit(0);
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
	if (!cmd || !cmd->path)
		return;
	if (!cmd->next)
		execute_simple_command(cmd, env);
	else
		execute_piped_commands(cmd, *env);
}