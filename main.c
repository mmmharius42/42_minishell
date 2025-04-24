/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 19:23:01 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/25 01:16:37 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_input(void)
{
	char	*input;

	input = custom_reader();
	return (input);
}

static void	process_input(char *input, t_env **env)
{
	t_token	*tokens;

	if (!check_input(input))
		return ;
	tokens = tokenize(input, *env);
	clean_shell(input, tokens);
}

int	main(int argc, char **argv, char **envp)
{
	t_env	*env;
	char	*input;
	int		exit_status;

	(void) argc;
	(void) argv;
	env = NULL;
	env_init(&env, envp);
	exit_status = 0;
	while (!exit_status)
	{
		input = get_input();
		if (!input)
			break ;
		if (*input)
			process_input(input, &env);
	}
	free_env(env);
	printf("exit\n");
	return (0);
}
