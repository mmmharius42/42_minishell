/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_operator.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 20:42:17 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/24 20:42:32 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*handle_pipe_operator(int *i)
{
	(*i)++;
	return (ft_strdup("|"));
}

static char	*handle_input_operator(char *input, int *i)
{
	if (input[*i + 1] == '<')
	{
		(*i) += 2;
		return (ft_strdup("<<"));
	}
	(*i)++;
	return (ft_strdup("<"));
}

static char	*handle_output_operator(char *input, int *i)
{
	if (input[*i + 1] == '>')
	{
		(*i) += 2;
		return (ft_strdup(">>"));
	}
	(*i)++;
	return (ft_strdup(">"));
}

char	*extract_operator(char *input, int *i)
{
	if (input[*i] == '|')
		return (handle_pipe_operator(i));
	else if (input[*i] == '<')
		return (handle_input_operator(input, i));
	else if (input[*i] == '>')
		return (handle_output_operator(input, i));
	return (NULL);
}
