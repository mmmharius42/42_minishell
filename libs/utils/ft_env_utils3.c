/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 01:36:35 by mpapin            #+#    #+#             */
/*   Updated: 2025/04/28 12:56:34 by mpapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sort_env(t_env **var, int size)
{
	int		i;
	int		j;
	t_env	*tmp;

	i = 0;
	while (i < size - 1)
	{
		j = 0;
		while (j < size - 1 - i)
		{
			if (ft_strcmp(var[j]->name, var[j + 1]->name) > 0)
			{
				tmp = var[j];
				var[j] = var[j + 1];
				var[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

int	env_len(t_env *env)
{
	int	len;

	len = 0;
	while (env)
	{
		len++;
		env = env->next;
	}
	return (len);
}

char	*clean_quotes(char *value)
{
	char	*cleaned;
	size_t	len;
	size_t	i;

	len = ft_strlen(value);
	cleaned = malloc(len - 1);
	if (!cleaned)
		return (NULL);
	i = 1;
	while (i < len - 1)
	{
		cleaned[i - 1] = value[i];
		i++;
	}
	cleaned[len - 2] = '\0';
	return (cleaned);
}

t_env	*create_env_node(const char *name, const char *value)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->name = ft_strdup(name);
	new->value = ft_strdup(value);
	new->equal_sign = 1;
	return (new);
}
