/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buitlin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 19:46:45 by mpapin            #+#    #+#             */
/*   Updated: 2025/04/24 19:46:45 by mpapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_builtin(t_cmd *cmd)
{
	if (!cmd->args[0])
		return (0);
	if (ft_strncmp(cmd->args[0], "history", 8) == 0)
		return (1);
	else if (ft_strncmp(cmd->args[0], "pwd", 4) == 0)
		return (1);
	else if (ft_strncmp(cmd->args[0], "echo", 5) == 0)
		return (1);
	else if (ft_strncmp(cmd->args[0], "env", 4) == 0)
		return (1);
	else if (ft_strncmp(cmd->args[0], "export", 7) == 0)
		return (1);
	else if (ft_strncmp(cmd->args[0], "unset", 6) == 0)
		return (1);
	else if (ft_strncmp(cmd->args[0], "cd", 6) == 0)
		return (1);
	else if (ft_strncmp(cmd->args[0], "exit", 6) == 0)
		return (1);
	return (0);
}

void	exec_builtin(t_cmd *cmd, t_env **env)
{
	if (!cmd->args[0])
		return ;
	if (ft_strncmp(cmd->args[0], "history", 8) == 0)
		ft_history();
	else if (ft_strncmp(cmd->args[0], "pwd", 4) == 0)
		ft_pwd();
	else if (ft_strncmp(cmd->args[0], "echo", 5) == 0)
		ft_echo(cmd->args);
	else if (ft_strncmp(cmd->args[0], "env", 4) == 0)
		ft_env(*env);
	else if (ft_strncmp(cmd->args[0], "export", 7) == 0)
		ft_export(cmd->args, env);
	else if (ft_strncmp(cmd->args[0], "unset", 6) == 0)
		ft_unset(cmd, env);
	else if (ft_strncmp(cmd->args[0], "cd", 3) == 0)
		ft_cd(cmd, env);
	else if (ft_strncmp(cmd->args[0], "exit", 5) == 0)
		ft_exit(cmd, env);
}
