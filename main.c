/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 19:23:01 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/28 18:42:13 by aberenge         ###   ########.fr       */
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
	save_history(input);
	tokens = tokenize(input, *env);
	if (tokens)
	{
		expand_all(tokens, *env);
		cmd = parse(tokens, env);
		free_tokens(tokens);
		if (cmd)
			exec(cmd, env);
		clean_shell(input, NULL, cmd);
	}
	else
		free(input);
}

static void	initialize_shell(t_env **ptr_env, char **envp)
{
	env_init(ptr_env, envp);
	load_history();
	setup_signals_interactive();
}

static void	run_shell_loop(t_env **ptr_env)
{
	char	*input;

	while (1)
	{
		input = get_input();
		if (!input)
			break ;
		if (*input)
			process_input(input, ptr_env);
		else
			free(input);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_env			*env;
	static t_env	**ptr_env;

	(void) argc;
	(void) argv;
	env = NULL;
	ptr_env = &env;
	initialize_shell(ptr_env, envp);
	run_shell_loop(ptr_env);
	free_env(env);
	cleanup_heredoc();
	printf("exit\n");
	return (g_return_code);
}
