/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 20:25:11 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/24 21:06:00 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_tokens(t_token *tokens)
{
	t_token	*current;
	char	*type_str;
	char	*quote_type;

	current = tokens;
	while (current)
	{
		if (current->type == WORD)
			type_str = "WORD";
		else if (current->type == PIPE)
			type_str = "PIPE";
		else if (current->type == REDIR_IN)
			type_str = "REDIR_IN";
		else if (current->type == REDIR_OUT)
			type_str = "REDIR_OUT";
		else if (current->type == APPEND)
			type_str = "APPEND";
		else if (current->type == HEREDOC)
			type_str = "HEREDOC";
		else
			type_str = "UNKNOWN";
		if (current->is_single_quote)
			quote_type = "SINGLE";
		else if (current->is_double_quote)
			quote_type = "DOUBLE";
		else
			quote_type = "NONE";
	}
  printf("Token: [%s] - Type: %s\n", current->value, type_str);
	current = current->next;
}

int	token_count(t_token *tokens)
{
	int		count;
	t_token	*current;

	count = 0;
	current = tokens;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}
