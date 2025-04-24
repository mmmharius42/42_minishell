/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 20:17:12 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/24 21:08:03 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_word(char *input, int *i)
{
	int		start;
	int		len;
	char	*word;
	char	quote;

	start = *i;
	if (input[*i] == '\'' || input[*i] == '"')
	{
		quote = input[(*i)++];
		start = *i;
		while (input[*i] && input[*i] != quote)
			(*i)++;
		len = *i - start;
		(*i)++;
	}
	else
	{
		while (input[*i] && !is_special_char(input[*i]))
			(*i)++;
		len = *i - start;
	}
	word = ft_substr(input, start, len);
	return (word);
}

t_token	*tokenize(char *input)
{
	int		i;
	t_token	*tokens;
	t_token	*new_token;

	if (!input)
		return (NULL);
	tokens = NULL;
	i = 0;
	while (input[i])
	{
		skip_spaces(input, &i);
		if (!input[i])
			break ;
		new_token = process_token(input, &i);
		if (!new_token)
		{
			free_tokens(tokens);
			return (NULL);
		}
		add_token(&tokens, new_token);
	}
	return (tokens);
}
