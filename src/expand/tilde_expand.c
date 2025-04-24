/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilde_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 10:12:34 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/25 00:55:11 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*process_tilde(char *str, t_env *env)
{
	char	*home;
	char	*result;

	if (!str || str[0] != '~')
		return (ft_strdup(str));
	home = get_env_value(env, "HOME");
	if (!home)
		return (ft_strdup(str));
	if (str[1] == '\0')
		return (ft_strdup(home));
	if (str[1] == '/')
	{
		result = ft_strjoin(home, str + 1);
		return (result);
	}
	return (ft_strdup(str));
}

void	expand_tilde(t_token *tokens, t_env *env)
{
	t_token	*current;
	char	*expanded;

	current = tokens;
	while (current)
	{
		if (current->type == WORD && !current->is_single_quote)
		{
			expanded = process_tilde(current->value, env);
			free(current->value);
			current->value = expanded;
		}
		current = current->next;
	}
}
