/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 18:45:51 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/28 19:15:51 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Recherche tous les tokens de type HEREDOC et les remplace par des REDIR_IN
** qui pointent vers un fichier temporaire contenant l'entrée du heredoc
*/
void	expand_heredocs(t_token *tokens, t_env *env)
{
	t_token	*current;
	t_token	*next;

	current = tokens;
	while (current && current->next)
	{
		if (current->type == HEREDOC)
		{
			next = current->next;
			if (!process_heredoc_token(current, next, env, tokens))
				return ;
		}
		current = current->next;
	}
}
