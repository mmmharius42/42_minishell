/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 23:59:10 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/26 16:05:22 by mpapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*env_new_var(char *name, char *value, int equal_sign)
{
	t_env	*new_var;

	new_var = malloc(sizeof(t_env));
	if (new_var == NULL)
		return (NULL);
	if (name != NULL)
		new_var->name = ft_strdup(name);
	else
		new_var->name = NULL;
	if (value != NULL)
		new_var->value = ft_strdup(value);
	else
		new_var->value = NULL;
	new_var->equal_sign = equal_sign;
	new_var->next = NULL;
	return (new_var);
}

void	env_add_back(t_env **env, t_env *new)
{
	t_env	*tmp;

	if (env == NULL || new == NULL)
		return ;
	if (*env == NULL)
	{
		*env = new;
		return ;
	}
	tmp = *env;
	while (tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = new;
}

void	env_init(t_env **env_list, char **env)
{
	int	i;
	char	*name;
	char	*value;
	int	equal;
	t_env	*new;

	i = -1;
	while (env[++i] != NULL)
	{
		equal = (ft_strchr(env[i], '=') != NULL);
		name = get_name(env[i]);
		value = get_value(env[i]);
		new = env_new_var(name, value, equal);
		free(name);
		free(value);
		env_add_back(env_list, new);
	}
}

char	*get_name(char *str)
{
	int		i;
	char	*name;

	i = 0;
	while (str[i] != '\0' && str[i] != '=')
		i++;
	name = malloc(i + 1);
	if (name == NULL)
		return (NULL);
	i = 0;
	while (str[i] != '\0' && str[i] != '=')
	{
		name[i] = str[i];
		i++;
	}
	name[i] = '\0';
	return (name);
}

char	*get_value(char *str)
{
	char	*equal;
	char	*value;
	char	*cleaned;

	equal = ft_strchr(str, '=');
	if (!equal)
		return (NULL);
	value = ft_strdup(equal + 1);
	if (!value)
		return (NULL);
	if (value[0] == '\"' && ft_strlen(value) >= 2
		&& value[ft_strlen(value) - 1] == '\"')
	{
		cleaned = clean_quotes(value);
		free(value);
		return (cleaned);
	}
	return (value);
}
