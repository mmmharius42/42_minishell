/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 19:53:35 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/28 18:26:33 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*print_path(void)
{
	char	buffer[1024];
	char	*pwd;
	char	*last_slash;
	char	*result;

	pwd = getcwd(buffer, sizeof(buffer));
	if (!pwd)
		return (ft_strdup("no_path ➜  "));
	last_slash = pwd + ft_strlen(pwd);
	while (last_slash != pwd && *last_slash != '/')
		last_slash--;
	if (*last_slash == '/')
		last_slash++;
	if (*last_slash == '\0')
		last_slash = "/";
	result = malloc(ft_strlen(last_slash) + ft_strlen(RED)
			+ ft_strlen(" ➜  ") + ft_strlen(RESET) + 1);
	if (!result)
		return (NULL);
	ft_strcpy(result, last_slash);
	ft_strcat(result, " ");
	ft_strcat(result, RED);
	ft_strcat(result, "➜  ");
	ft_strcat(result, RESET);
	return (result);
}

char	*custom_reader(void)
{
	char	*input;
	char	*line;

	line = print_path();
	input = readline("> ");
	free(line);
	return (input);
}
