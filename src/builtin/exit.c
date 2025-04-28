/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 13:47:20 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/28 13:47:20 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	ft_exit(t_cmd *cmd, t_env **env)
{
	long	status;
	int		arg_count;

	ft_putendl_fd("exit", STDOUT_FILENO);
	arg_count = 0;
	status = g_return_code;
	while (cmd->args[arg_count])
		arg_count++;
	if (arg_count > 1)
	{
		if (!is_numeric(cmd->args[1]))
		{
			ft_putendl_fd("minishell: exit: numeric argument required", 2);
			status = 255;
		}
		else if (arg_count > 2)
		{
			ft_putendl_fd("minishell: exit: too many arguments", 2);
			g_return_code = 1;
			return ;
		}
		else
			status = ft_atoi(cmd->args[1]);
	}
	free_env(*env);
	rl_clear_history();
	exit((unsigned char)status);
}