/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 17:47:09 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/27 21:16:42 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_piped_child(t_cmd *cmd, t_env *env, int prev_pipe, int *pipe_fd)
{
	handle_input_pipe(prev_pipe);
	if (cmd->next)
		handle_output_pipe(pipe_fd);
	if (cmd->redir && !apply_redirections(cmd->redir))
		exit(g_return_code);
	if (check_builtin(cmd))
		exec_builtin(cmd, &env);
	else
		execute_command(cmd, env);
	close(0);
	close(1);
	close(2);
	exit(0);
}

void	exec(t_cmd *cmd, t_env **env)
{
	if (!cmd)
		return ;
	if (!cmd->path && cmd->redir)
	{
		handle_redir_only(cmd);
		return ;
	}
	if (!cmd->path)
		return ;
	setup_signals_parent();
	if (!cmd->next)
		execute_simple_command(cmd, env);
	else
		execute_piped_commands(cmd, *env);
	setup_signals_interactive();
}
