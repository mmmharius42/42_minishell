/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 23:48:23 by mpapin            #+#    #+#             */
/*   Updated: 2025/04/25 01:39:13 by mpapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_arg(char *arg)
{
	int	i;

	if (!arg || (!isalpha(arg[0]) && arg[0] != '_'))
	{
		printf("bash: export: `%s': not a valid identifier\n", arg);
		return (0);
	}
	i = 1;
	while (arg[i] && arg[i] != '=')
	{
		if (!isalnum(arg[i]) && arg[i] != '_')
		{
			printf("bash: export: `%s': not a valid identifier\n", arg);
			return (0);
		}
		i++;
	}
	return (1);
}

void	print_export_line(t_env *var)
{
	printf("declare -x %s", var->name);
	if (var->equal_sign)
	{
		printf("=\"");
		if (var->value)
			printf("%s", var->value);
		printf("\"\n");
	}
	else
		printf("\n");
}

void	export_sort_print(t_env *env)
{
	int		i;
	int		len;
	t_env	**arr;

	len = env_len(env);
	arr = malloc(sizeof(t_env *) * len);
	if (!arr)
		return ;
	i = 0;
	while (env)
	{
		arr[i] = env;
		env = env->next;
		i++;
	}
	sort_env(arr, len);
	i = 0;
	while (i < len)
	{
		print_export_line(arr[i]);
		i++;
	}
	free(arr);
}

void	handle_export_arg(char *arg, t_env **env)
{
	char	*name;
	char	*value;
	t_env	*new;
	int		equal;

	if (!check_arg(arg))
		return ;
	equal = 0;
	if (strchr(arg, '=') != NULL)
		equal = 1;
	name = get_name(arg);
	value = get_value(arg);
	if (var_exists(*env, name))
		update_var(*env, name, value, equal);
	else
	{
		new = env_new_var(name, value, equal);
		env_add_back(env, new);
	}
	free(name);
	free(value);
}

void	ft_export(char **args, t_env **env)
{
	int	i;

	if (!args[1])
	{
		export_sort_print(*env);
		return ;
	}
	i = 0;
	while (args[++i])
		handle_export_arg(args[i], env);
}
