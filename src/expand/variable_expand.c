/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expand.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 23:27:14 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/24 23:51:52 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*process_variable(char *str, char *input, int *i)
{
	char	*var_name;

	var_name = get_var_name(&input[*i]);
	*i += strlen(var_name) + 1;
	return (replace_var(str, var_name));
}

char	*replace_variables(char *input)
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
		if (input[i] == '$')
			new_str = process_variable(new_str, input, &i);
		else
		{
			new_str = add_char_to_str(new_str, input[i]);
			i++;
		}
	}
	return (new_str);
}

void	expand_variable(t_token *tokens)
{
	char	*expanded;

	while (tokens)
	{
		expanded = replace_variables(tokens->value);
		printf("Expanded: %s\n", expanded);
		free(expanded);
		tokens = tokens->next;
	}
}
