/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 19:35:33 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/24 19:48:28 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_quote(char *input)
{
	int	i;
	int	double_quotes;
	int	single_quotes;

	i = -1;
	single_quotes = 0;
	double_quotes = 0;
	while (input[++i])
	{
		if (input[i] == '"')
			double_quotes++;
		if (input[i] == '\'')
			single_quotes++;
	}
	if ((!(double_quotes % 2 == 0) && double_quotes != 0)
		|| (!(single_quotes % 2 == 0) && single_quotes != 0))
		return (printf("Minishell: Unclosed quotes\n"), 0);
	return (1);
}

int	check_input(char *input)
{
	if (!check_quote(input))
		return (0);
	return (1);
}
