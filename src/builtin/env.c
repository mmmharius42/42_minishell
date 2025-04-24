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

void	ft_env(t_env *env_list)
{
	while (env_list != NULL)
	{
		if (env_list->equal_sign && env_list->value != NULL)
			printf("%s=%s\n", env_list->name, env_list->value);
		env_list = env_list->next;
	}
}
