/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 11:03:18 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/28 13:05:09 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Nettoie le fichier heredoc
*/
static void	clean_heredoc_file(void)
{
	char	filename[PATH_MAX];
	char	*home;

	home = getenv("HOME");
	if (!home)
		home = "/tmp";
	ft_strcpy(filename, home);
	ft_strcat(filename, "/.minishell_heredoc");
	unlink(filename);
}

void	free_child(t_cmd *cmd, t_env *env)
{
	free_commands(cmd);
	free_env(env);
	rl_clear_history();
	clean_heredoc_file();
	close(0);
	close(1);
	close(2);
}
