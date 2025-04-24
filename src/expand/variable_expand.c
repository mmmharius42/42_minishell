/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expand.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 23:27:14 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/25 01:14:00 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*process_variable(char *str, char *input, int *i, t_env *env)
{
	char	*var_name;
	char	*result;

	var_name = get_var_name(&input[*i]);
	*i += ft_strlen(var_name) + 1;
	result = replace_var(str, var_name, env);
	return (result);
}

static char	*replace_all_variables(char *input, t_env *env)
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
			new_str = process_variable(new_str, input, &i, env);
		else
		{
			new_str = add_char_to_str(new_str, input[i]);
			i++;
		}
	}
	return (new_str);
}

void	expand_variable(t_token *tokens, t_env *env)
{
	char	*expanded;
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (current->is_single_quote && !current->is_double_quote)
		{
			current = current->next;
			continue ;
		}
		expanded = replace_all_variables(current->value, env);
		free(current->value);
		current->value = expanded;
		current = current->next;
	}
}
