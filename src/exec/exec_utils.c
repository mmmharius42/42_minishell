/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 21:30:00 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/28 15:06:31 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_return_code = 0;

char	**env_to_array(t_env *env)
{
	t_env	*current;
	char	**env_array;
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
	return (fill_env_array(env, env_array));
}

char	**fill_env_array(t_env *env, char **env_array)
{
	t_env	*current;
	char	*tmp;
	int		i;

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

void	execute_command(t_cmd *cmd, t_env *env)
{
	char	**env_array;

	setup_signals_child();
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
		free_child(cmd, env);
		exit(127);
	}
}

void	handle_builtin(t_cmd *cmd, t_env **env)
{
	int	stdin_backup;
	int	stdout_backup;

	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);
	if (cmd->redir && !apply_redirections(cmd->redir))
	{
		g_return_code = 1;
		return ;
	}
	exec_builtin(cmd, env);
	dup2(stdin_backup, STDIN_FILENO);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdin_backup);
	close(stdout_backup);
}

void	handle_fork_error(void)
{
	g_return_code = 1;
}
