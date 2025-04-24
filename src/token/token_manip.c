/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_manip.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 20:22:35 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/24 21:04:33 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*create_token(char *value, int type, int is_single_quote,
						int is_double_quote)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(value);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	token->type = type;
	token->is_single_quote = is_single_quote;
	token->is_double_quote = is_double_quote;
	token->next = NULL;
	return (token);
}

void	add_token(t_token **tokens, t_token *new_token)
{
	t_token	*current;

	if (!tokens || !new_token)
		return ;
	if (!*tokens)
	{
		*tokens = new_token;
		return ;
	}
	current = *tokens;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

t_token	*process_token(char *input, int *i)
{
	char	*value;
	t_token	*token;
	int		type;
	int		is_single_quote;
	int		is_double_quote;

	is_single_quote = 0;
	is_double_quote = 0;
	if (!input[*i])
		return (NULL);
	if (input[*i] == '|' || input[*i] == '<' || input[*i] == '>')
		value = extract_operator(input, i);
	else
		value = extract_word(input, i, &is_single_quote, &is_double_quote);
	if (!value)
		return (NULL);
	type = get_token_type(value);
	token = create_token(value, type, is_single_quote, is_double_quote);
		value = extract_word(input, i);
	if (!value)
		return (NULL);
	type = get_token_type(value);
	token = create_token(value, type);
	free(value);
	return (token);
}
