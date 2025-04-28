/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 21:30:00 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/27 21:16:39 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_input_pipe(int prev_pipe)
{
	if (prev_pipe != -1)
	{
		dup2(prev_pipe, STDIN_FILENO);
		close(prev_pipe);
	}
}

void	handle_output_pipe(int *pipe_fd)
{
	close(pipe_fd[0]);
	dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[1]);
}

void	update_pipe_status(int *prev_pipe, int *pipe_fd, t_cmd *current)
{
	if (*prev_pipe != -1)
		close(*prev_pipe);
	if (current->next)
	{
		close(pipe_fd[1]);
		*prev_pipe = pipe_fd[0];
	}
	else
		*prev_pipe = -1;
}

void	execute_piped_commands(t_cmd *cmd_list, t_env *env)
{
	pid_t	pid;
	int		pipe_fd[2];
	int		prev_pipe;
	t_cmd	*current;

	current = cmd_list;
	prev_pipe = -1;
	while (current)
	{
		if (current->next && pipe(pipe_fd) == -1)
			return ;
		pid = fork();
		if (pid == -1)
			return ;
		if (pid == 0)
			execute_piped_child(current, env, prev_pipe, pipe_fd);
		update_pipe_status(&prev_pipe, pipe_fd, current);
		current = current->next;
	}
	wait_for_all_children(cmd_list);
}

void	wait_for_all_children(t_cmd *cmd_list)
{
	int		status;
	t_cmd	*current;

	current = cmd_list;
	while (current)
	{
		waitpid(-1, &status, 0);
		if (WIFEXITED(status))
			g_return_code = WEXITSTATUS(status);
		current = current->next;
	}
}
