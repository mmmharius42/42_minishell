/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 16:54:39 by mpapin            #+#    #+#             */
/*   Updated: 2025/04/26 15:42:24 by mpapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_pwd_value(void)
{
	char	*pwd;
	char	*prefix;
	char	*pwd_value;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (NULL);
	prefix = ft_strdup("PWD=");
	if (!prefix)
	{
		free(pwd);
		return (NULL);
	}
	pwd_value = ft_strjoin(prefix, pwd);
	free(pwd);
	free(prefix);
	return (pwd_value);
}

void	export_variable(const char *name, const char *value, t_env **env)
{
	t_env *tmp = *env;
	while (tmp)
	{
		if (ft_strcmp(tmp->name, name) == 0)
		{
			free(tmp->value);
			tmp->value = ft_strdup(value);
			tmp->equal_sign = 1;
			return;
		}
		tmp = tmp->next;
	}
	t_env *new = malloc(sizeof(t_env));
	if (!new)
		return;
	new->name = ft_strdup(name);
	new->value = ft_strdup(value);
	new->equal_sign = 1;
	new->next = *env;
	*env = new;
}

char	*get_target_path(t_cmd *cmd, t_env *env)
{
	char	*home;

	if (!cmd->args[1] || ft_strcmp(cmd->args[1], "~") == 0 ||
		ft_strcmp(cmd->args[1], "~/") == 0)
	{
		home = get_env_value(env, "HOME");
		if (!home)
		{
			printf("cd: HOME not set\n");
			return (NULL);
		}
		return (ft_strdup(home));
	}
	return (ft_strdup(cmd->args[1]));
}

void	update_pwd_env(t_env **env, char *old_pwd)
{
	char	*new_pwd;

	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
		return ;
	export_variable("OLDPWD", old_pwd, env);
	export_variable("PWD", new_pwd, env);
	free(new_pwd);
}

void	ft_cd(t_cmd *cmd, t_env **env)
{
	char	*current_pwd;
	char	*target_path;

	current_pwd = getcwd(NULL, 0);
	if (!current_pwd)
	{
		printf("cd: error retrieving current directory\n");
		return ;
	}
	target_path = get_target_path(cmd, *env);
	if (!target_path)
	{
		free(current_pwd);
		return ;
	}
	if (chdir(target_path) == -1)
	{
		printf("cd: %s: %s\n", target_path, strerror(errno));
		free(current_pwd);
		free(target_path);
		return ;
	}
	update_pwd_env(env, current_pwd);
	free(current_pwd);
	free(target_path);
}
