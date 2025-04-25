/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 01:31:02 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/25 02:45:55 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_error(char *message)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(message, 2);
	ft_putstr_fd("\n", 2);
	return (0);
}

int	is_redirection(int type)
{
	return (type == REDIR_IN || type == REDIR_OUT
		|| type == APPEND || type == HEREDOC);
}

void	add_token_to_cmd(t_token *token, t_cmd *cmd)
{
	t_token	*new_token;

	new_token = create_token(token->value, token->type,
			token->is_single_quote, token->is_double_quote);
	if (!new_token)
		return ;
	add_token(&(cmd->tokens), new_token);
}
