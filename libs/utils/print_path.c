/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 19:53:35 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/24 20:07:38 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*print_path(void)
{
	char	buffer[1024];
	char	*pwd;
	char	*last_slash;
	char	*result;

	pwd = getcwd(buffer, sizeof(buffer));
	if (!pwd)
		return (ft_strdup("no_path "RED"➜"RESET"  "));
	last_slash = pwd + strlen(pwd);
	while (last_slash != pwd && *last_slash != '/')
		last_slash--;
	if (*last_slash == '/')
		last_slash++;
	result = malloc(ft_strlen(last_slash)+ (ft_strlen(RED) + ft_strlen(RESET))
			+ 6);
	if (!result)
		return (NULL);
	result[0] = '\0';
	ft_strcat(result, last_slash);
	ft_strcat(result, " "RED"➜"RESET"  ");
	return (result);
}
