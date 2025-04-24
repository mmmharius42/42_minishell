/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 23:48:23 by mpapin            #+#    #+#             */
/*   Updated: 2025/04/25 01:27:40 by mpapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <signal.h>
# include <sys/stat.h>

typedef struct s_cmd
{
	char	*path;
	char	**args;
}	t_cmd;

typedef struct s_env
{
	char			*name;
	char			*value;
	int				equal_sign;
	struct s_env	*next;
}	t_env;

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
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

	equal = strchr(str, '=');
	if (equal == NULL)
		return (NULL);
	value = strdup(equal + 1);
	return (value);
}

t_env    *env_new_var(char *name, char *value, int equal_sign)
{
	t_env    *new_var;

	new_var = malloc(sizeof(t_env));
	if (new_var == NULL)
		return (NULL);
	if (name != NULL)
		new_var->name = strdup(name);
	else
		new_var->name = NULL;
	if (value != NULL)
		new_var->value = strdup(value);
	else
		new_var->value = NULL;
	new_var->equal_sign = equal_sign;
	new_var->next = NULL;
	return (new_var);
}

void    env_add_back(t_env **env, t_env *new)
{
	t_env    *tmp;

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

void    env_init(t_env **env_list, char **env)
{
	int        i;
	char    *name;
	char    *value;
	int        equal;
	t_env    *new;

	i = -1;
	while (env[++i] != NULL)
	{
		equal = (strchr(env[i], '=') != NULL);
		name = get_name(env[i]);
		value = get_value(env[i]);
		new = env_new_var(name, value, equal);
		free(name);
		free(value);
		env_add_back(env_list, new);
	}
}

void    free_env(t_env *env)
{
	t_env    *tmp;

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

static int	var_exists(t_env *env, char *var)
{
	while (env != NULL)
	{
		if (ft_strcmp(env->name, var) == 0)
			return (1);
		env = env->next;
	}
	return (0);
}


//

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
		arr[i++] = env;
		env = env->next;
	}
	sort_env(arr, len);
	i = 0;
	while (i < len)
	{
		if (arr[i]->equal_sign)
		{
			printf("declare -x %s=", arr[i]->name);
			if (arr[i]->value)
				printf("\"%s\"\n", arr[i]->value);
			else
				printf("\"%s\"\n", "");
		}
		else
			printf("declare -x %s\n", arr[i]->name);
		i++;
	}
	free(arr);
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
				env->value = value ? strdup(value) : NULL;
				env->equal_sign = 1;
			}
			return ;
		}
		env = env->next;
	}
}

void	ft_export(char **args, t_env **env)
{
	int		i;
	char	*name;
	char	*value;
	int		equal;
	t_env	*new;

	if (!args[1])
	{
		export_sort_print(*env);
		return ;
	}
	i = 0;
	while (args[++i])
	{
		if (!check_arg(args[i]))
			continue ;
		equal = (strchr(args[i], '=') != NULL);
		name = get_name(args[i]);
		value = get_value(args[i]);
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
}


int main(int argc, char **argv, char **envp)
{
	t_env *env_list = NULL;
	t_cmd *cmd = malloc(sizeof(t_cmd));

	if (!cmd)
		return (1);
	env_init(&env_list, envp);
	cmd->args = malloc(sizeof(char *) * 3);
	if (!cmd->args)
		return (free(cmd), 1);
	cmd->args[0] = strdup("export");
	cmd->args[1] = strdup("_TEST=hello");
	cmd->args[2] = NULL;
	ft_export(cmd->args, &env_list);
	cmd->args[1] = NULL;
	free(cmd->args[0]);
	ft_export(cmd->args, &env_list);
	free(cmd->args[1]);
	free(cmd->args);
	free(cmd);
	free_env(env_list);
	return (0);
}
