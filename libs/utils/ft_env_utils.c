/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 19:45:47 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/25 01:19:30 by mpapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(t_env *env_list, char *name)
{
	while (env_list)
	{
		if (ft_strcmp(env_list->name, name) == 0)
			return (env_list->value);
		env_list = env_list->next;
	}
	return (NULL);
}

void	set_env_value(t_env *env_list, char *name, char *value)
{
	while (env_list)
	{
		if (ft_strcmp(env_list->name, name) == 0)
		{
			if (env_list->value)
				free(env_list->value);
			env_list->value = ft_strdup(value);
			env_list->equal_sign = 1;
			return;
		}
		env_list = env_list->next;
	}
	t_env *new = env_new_var(name, value, 1);
	env_add_back(&env_list, new);
}

void	unset_env_var(t_env **env_list, char *name)
{
	t_env *current;
	t_env *prev;

	if (!env_list || !*env_list)
		return;

	current = *env_list;
	prev = NULL;
	if (ft_strcmp(current->name, name) == 0)
	{
		*env_list = current->next;
		free(current->name);
		if (current->value)
			free(current->value);
		free(current);
		return;
	}
	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
		{
			prev->next = current->next;
			free(current->name);
			if (current->value)
				free(current->value);
			free(current);
			return;
		}
		prev = current;
		current = current->next;
	}
}

int	var_exists(t_env *env, char *var)
{
	while (env != NULL)
	{
		if (ft_strcmp(env->name, var) == 0)
			return (1);
		env = env->next;
	}
	return (0);
}

