/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 21:28:14 by mpapin            #+#    #+#             */
/*   Updated: 2025/04/24 23:47:09 by mpapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		ft_putstr_fd(cwd, 1);
		write(1, "\n", 1);
		free(cwd);
	}
	else
	{
		ft_putstr_fd("pwd: error retrieving current directory: getcwd: ", 2);
		ft_putstr_fd(strerror(errno), 2);
		write(2, "\n", 1);
	}
}
