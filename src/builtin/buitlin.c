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

extern int	g_return_code;

int	check_builtin(t_cmd *cmd)
{
	if (!cmd->args || !cmd->args[0])
		return (0);
	if (ft_strcmp(cmd->args[0], "echo") == 0 || ft_strcmp(cmd->args[0], "cd") == 0
		|| ft_strcmp(cmd->args[0], "pwd") == 0 || ft_strcmp(cmd->args[0], "export") == 0
		|| ft_strcmp(cmd->args[0], "unset") == 0 || ft_strcmp(cmd->args[0], "env") == 0
		|| ft_strcmp(cmd->args[0], "exit") == 0)
		return (1);
	return (0);
}

void	exec_builtin(t_cmd *cmd, char ***env_ptr)
{
	// Temporaire - la commande est reconnue comme builtin mais n'est pas implémentée
	ft_putstr_fd("builtin command recognized: ", 1);
	ft_putstr_fd(cmd->args[0], 1);
	ft_putstr_fd("\n", 1);
	g_return_code = 0;
	(void)env_ptr;
}