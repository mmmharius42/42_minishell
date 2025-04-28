/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 19:23:01 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/28 12:45:19 by mpapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_input(void)
{
	char	*input;

	input = custom_reader();
	if (input && *input)
		add_history(input);
	return (input);
}

static void	process_input(char *input, t_env **env)
{
	t_token	*tokens;
	t_cmd	*cmd;

	if (!check_input(input) || !(*input))
		return ;
	tokens = tokenize(input, *env);
	cmd = parse(tokens, env);
	if (cmd)
		exec(cmd, env);
	clean_shell(input, tokens, cmd);
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
	load_history();
	setup_signals_interactive();
	while (1)
	{
		input = get_input();
		if (!input)
			break ;
		if (*input)
		{
			save_history(input);
			process_input(input, &env);
		}
		else
			free(input);
	}
	free_env(env);
	printf("exit\n");
	return (g_return_code);
}
