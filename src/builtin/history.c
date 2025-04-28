/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 01:41:20 by mpapin            #+#    #+#             */
/*   Updated: 2025/04/28 12:57:05 by mpapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_history_file_path(void)
{
	char	*home;
	char	*history_path;

	home = getenv("HOME");
	if (!home)
	{
		ft_putendl_fd("HOME environment variable not set", 1);
		return (NULL);
	}
	history_path = malloc(strlen(home) + strlen("/.minishell_history") + 1);
	if (!history_path)
	{
		ft_putendl_fd("Memory allocation failed", 1);
		return (NULL);
	}
	ft_strcpy(history_path, home);
	ft_strcat(history_path, "/.minishell_history");
	return (history_path);
}

void	load_history(void)
{
	int		fd;
	char	*line;
	char	*history_file;

	history_file = get_history_file_path();
	if (!history_file)
		return ;
	fd = open(history_file, O_RDONLY);
	if (fd == -1)
	{
		free(history_file);
		return ;
	}
	line = get_next_line(fd);
	while (line)
	{
		add_history(line);
		free(line);
		line = get_next_line(fd);
	}
	free(history_file);
	close(fd);
}

void	save_history(char *input)
{
	int		fd;
	char	*history_file;

	if (!input || !*input || ft_strnstr(input, "<<", 2))
		return ;
	history_file = get_history_file_path();
	if (!history_file)
		return ;
	fd = open(history_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		free(history_file);
		return ;
	}
	write(fd, input, ft_strlen(input));
	write(fd, "\n", 1);
	free(history_file);
	close(fd);
}

void	ft_history(void)
{
	char	*line;
	int		fd;
	char	*history_file;

	history_file = get_history_file_path();
	if (!history_file)
		return ;
	fd = open(history_file, O_RDONLY);
	if (fd == -1)
	{
		free(history_file);
		return ;
	}
	line = get_next_line(fd);
	while (line)
	{
		printf("%s", line);
		free(line);
		line = get_next_line(fd);
	}
	free(history_file);
	close(fd);
}
