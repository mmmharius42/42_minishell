/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 19:45:47 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/28 12:52:42 by mpapin           ###   ########.fr       */
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

static void	free_env_node(t_env *env)
{
	free(env->name);
	if (env->value)
		free(env->value);
	free(env);
}

void	unset_env_var(t_env **env_list, char *name)
{
	t_env	*current;
	t_env	*prev;

	if (!env_list || !*env_list)
		return ;
	current = *env_list;
	prev = NULL;
	if (ft_strcmp(current->name, name) == 0)
	{
		*env_list = current->next;
		free_env_node(current);
		return ;
	}
	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
		{
			prev->next = current->next;
			free_env_node(current);
			return ;
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

void	update_var(t_env *env, char *name, char *value, int equal)
{
	while (env)
	{
		if (ft_strcmp(env->name, name) == 0)
		{
			if (equal)
			{
				free(env->value);
				if (value)
					env->value = ft_strdup(value);
				else
					env->value = NULL;
				env->equal_sign = 1;
			}
			return ;
		}
		env = env->next;
	}
}
