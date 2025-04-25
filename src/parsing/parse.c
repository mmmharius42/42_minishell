/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 01:10:51 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/25 02:46:31 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*parse_tokens(t_token *tokens)
{
	t_cmd	*cmd_list;
	t_cmd	*current_cmd;
	t_token	*current_token;

	cmd_list = NULL;
	current_cmd = create_command();
	if (!current_cmd)
		return (NULL);
	current_token = tokens;
	while (current_token)
	{
		if (!process_token_to_cmd(current_token, &current_cmd, &cmd_list))
		{
			free_commands(cmd_list);
			free_command(current_cmd);
			return (NULL);
		}
		current_token = current_token->next;
	}
	if (current_cmd && (current_cmd->tokens || current_cmd->redir))
		add_command(&cmd_list, current_cmd);
	else if (current_cmd)
		free_command(current_cmd);
	return (cmd_list);
}

int	process_token_to_cmd(t_token *token, t_cmd **current_cmd, t_cmd **cmd_list)
{
	if (token->type == PIPE)
	{
		if (!(*current_cmd)->tokens)
			return (parse_error("syntax error near unexpected token `|'"));
		add_command(cmd_list, *current_cmd);
		*current_cmd = create_command();
		if (!(*current_cmd))
			return (0);
		return (1);
	}
	else if (is_redirection(token->type))
		return (process_redirection(token, current_cmd));
	else if (token->type == WORD)
		add_token_to_cmd(token, *current_cmd);
	return (1);
}

t_cmd	*parse(t_token *tokens, t_env **env)
{
	t_cmd	*cmd;

	if (!tokens)
		return (NULL);
	cmd = parse_tokens(tokens);
	if (!cmd)
		return (NULL);
	if (!prepare_all_args(cmd))
	{
		free_commands(cmd);
		return (NULL);
	}
	if (!resolve_paths(cmd, *env))
	{
		free_commands(cmd);
		return (NULL);
	}
	return (cmd);
}
