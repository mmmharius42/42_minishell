/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_code_expand.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 00:00:00 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/27 19:47:32 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*replace_exit_code(char *str)
{
	char	*result;
	char	*exit_code_str;

	exit_code_str = ft_itoa(g_return_code);
	if (!exit_code_str)
		return (str);
	result = ft_strjoin(str, exit_code_str);
	free(exit_code_str);
	free(str);
	return (result);
}

static char	*process_exit_code(char *str, char *input, int *i)
{
	(void) input;
	(*i) += 2;
	return (replace_exit_code(str));
}

static char	*replace_all_exit_codes(char *input)
{
	int		i;
	char	*new_str;

	i = 0;
	new_str = malloc(1);
	if (!new_str)
		return (NULL);
	new_str[0] = '\0';
	while (input[i])
	{
		if (input[i] == '$' && input[i + 1] == '?')
			new_str = process_exit_code(new_str, input, &i);
		else
		{
			new_str = add_char_to_str(new_str, input[i]);
			i++;
		}
	}
	return (new_str);
}

void	expand_exit_code(t_token *tokens)
{
	char	*expanded;
	t_token	*current;

	current = tokens;
	while (current)
	{
		expanded = replace_all_exit_codes(current->value);
		free(current->value);
		current->value = expanded;
		current = current->next;
	}
}
