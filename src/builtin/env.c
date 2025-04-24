/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 19:45:47 by mpapin            #+#    #+#             */
/*   Updated: 2025/04/24 19:45:47 by mpapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

	equal = ft_strchr(str, '=');
	if (equal == NULL)
		return (NULL);
	value = ft_strdup(equal + 1);
	return (value);
}

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
	int		i;
	char	*name;
	char	*value;
	int		equal;
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

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env != NULL)
	{
		tmp = env->next;
		if (env->name)
			free(env->name);
		if (env->value)
			free(env->value);
		free(env);
		env = tmp;
	}
}

void	ft_env(t_env *env_list)
{
	while (env_list != NULL)
	{
		if (env_list->equal_sign && env_list->value != NULL)
			printf("%s=%s\n", env_list->name, env_list->value);
		env_list = env_list->next;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_env *env_list = NULL;

	(void)argc;
	(void)argv;
	env_init(&env_list, envp);
	ft_env(env_list);
	free_env(env_list);
	return (0);
}
