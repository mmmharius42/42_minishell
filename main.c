/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 19:23:01 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/24 20:34:16 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_input(void)
{
	char	*input;

	input = custom_reader();
	return (input);
}

static void	process_input(char *input)
{
	t_token	*tokens;

	if (!check_input(input))
		return ;
	tokens = tokenize(input);
	print_tokens(tokens);
	clean_shell(input, tokens);
}

int	main(int argc, char **argv, char **env)
{
	char	*input;
	int		exit_status;

	(void) argc;
	(void) argv;
	(void) env;
	exit_status = 0;
	while (!exit_status)
	{
		input = get_input();
		if (!input)
			break ;
		if (*input)
			process_input(input);
	}
	printf("exit\n");
	return (0);
}
