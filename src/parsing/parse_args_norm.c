/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args_norm.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 18:36:46 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/28 18:37:27 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Compte le nombre de tokens de type WORD dans une liste de tokens.
** Retourne le nombre de mots.
*/
int	count_args(t_token *tokens)
{
	int		count;
	t_token	*current;

	count = 0;
	current = tokens;
	while (current)
	{
		if (current->type == WORD)
			count++;
		current = current->next;
	}
	return (count);
}

/*
** Remplit le tableau d'arguments à partir des tokens de type WORD.
** Retourne le tableau d'arguments.
*/
int	allocate_arg(char **args, int i, char *value)
{
	args[i] = ft_strdup(value);
	if (!args[i])
	{
		while (--i >= 0)
			free(args[i]);
		free(args);
		return (0);
	}
	return (1);
}
