/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_word.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 22:15:12 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/24 21:08:30 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*extract_quoted_word(char *input, int *i, int *is_single_quote,
								int *is_double_quote)
{
	int		start;
	int		len;
	char	quote;
	char	*word;

	quote = input[(*i)++];
	if (quote == '\'')
		*is_single_quote = 1;
	else
		*is_double_quote = 1;
	start = *i;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	len = *i - start;
	if (input[*i])
		(*i)++;
	word = ft_substr(input, start, len);
	return (word);
}

static char	*extract_unquoted_word(char *input, int *i)
{
	int		start;
	int		len;
	char	*word;

	start = *i;
	while (input[*i] && !is_special_char(input[*i]))
		(*i)++;
	len = *i - start;
	word = ft_substr(input, start, len);
	return (word);
}

char	*extract_word(char *input, int *i, int *is_single_quote,
					int *is_double_quote)
{
	if (input[*i] == '\'' || input[*i] == '"')
		return (extract_quoted_word(input, i, is_single_quote,
				is_double_quote));
	else
		return (extract_unquoted_word(input, i));
}
