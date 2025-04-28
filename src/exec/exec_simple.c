/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 21:30:00 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/27 21:16:06 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	wait_for_child(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		g_return_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_return_code = 128 + WTERMSIG(status);
}

void	execute_simple_command(t_cmd *cmd, t_env **env)
{
	pid_t	pid;

	if (check_builtin(cmd))
	{
		handle_builtin(cmd, env);
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		handle_fork_error();
		return ;
	}
	if (pid == 0)
	{
		execute_command(cmd, *env);
		exit(g_return_code);
	}
	else
		wait_for_child(pid);
}

void	handle_redir_only(t_cmd *cmd)
{
	pid_t	pid;
	int		status;

	setup_signals_parent();
	pid = fork();
	if (pid == 0)
	{
		setup_signals_child();
		if (!apply_redirections(cmd->redir))
			exit(g_return_code);
		exit(0);
	}
	else
	{
		waitpid(pid, &status, 0);
		setup_signals_interactive();
		if (WIFEXITED(status))
			g_return_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			g_return_code = 128 + WTERMSIG(status);
	}
}
