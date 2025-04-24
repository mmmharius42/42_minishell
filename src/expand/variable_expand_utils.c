/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expand_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 23:51:25 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/24 23:51:56 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_var_name(char *value)
{
	int		i;
	int		j;
	char	*var_name;

	i = 1;
	while (value[i] && ft_is_path(value[i]))
		i++;
	var_name = malloc(sizeof(char) * i);
	if (!var_name)
		return (NULL);
	j = 1;
	while (j < i)
	{
		var_name[j - 1] = value[j];
		j++;
	}
	var_name[i - 1] = '\0';
	return (var_name);
}

char	*replace_var(char *str, char *var_name)
{
	char	*result;
	char	*replacement;

	replacement = ft_strjoin("test", "");
	result = ft_strjoin(str, replacement);
	free(str);
	free(replacement);
	free(var_name);
	return (result);
}

char	*add_char_to_str(char *str, char c)
{
	char	*result;
	char	buf[2];

	buf[0] = c;
	buf[1] = '\0';
	result = ft_strjoin(str, buf);
	free(str);
	return (result);
}
