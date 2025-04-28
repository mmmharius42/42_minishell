/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 19:35:33 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/28 18:52:27 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_double_and(char *input)
{
	int	i;

	i = -1;
	while (input[++i])
	{
		if (input[i] == '&' && input[i + 1] == '&')
		{
			printf("minishell: syntax error near unexpected token `&&'\n");
			return (0);
		}
	}
	return (1);
}

static int	check_quote(char *input)
{
	int	i;
	int	in_double;
	int	in_single;

	i = 0;
	in_double = 0;
	in_single = 0;
	while (input[i])
	{
		if (input[i] == '"' && !in_single)
			in_double = !in_double;
		else if (input[i] == '\'' && !in_double)
			in_single = !in_single;
		i++;
	}
	if (in_double || in_single)
		return (printf("Minishell: Unclosed quotes\n"), 0);
	return (1);
}

int	check_input(char *input)
{
	if (!check_quote(input))
		return (0);
	if (!check_double_and(input))
		return (0);
	return (1);
}
