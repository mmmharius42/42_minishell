/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 19:49:04 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/28 18:42:20 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env != NULL)
	{
		tmp = env->next;
		if (env->name)
			free(env->name);
		if (env->value)
			free(env->value);
		free(env);
		env = tmp;
	}
}

void	free_tokens(t_token	*tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->value);
		free(tmp);
	}
}

void	clean_shell(char *input, t_token *tokens, t_cmd *cmd)
{
	(void) tokens;
	if (input)
		free(input);
	if (cmd)
		free_commands(cmd);
}

void	cleanup_heredoc(void)
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
