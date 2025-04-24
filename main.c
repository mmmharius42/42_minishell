/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 19:23:01 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/25 01:44:55 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    print_cmd_info(t_cmd *cmd)
{
    int     i;
    t_redir *redir;
    t_cmd   *current;
    int     cmd_count;

    if (!cmd)
    {
        printf("No command to display\n");
        return;
    }

    cmd_count = 0;
    current = cmd;
    while (current)
    {
        cmd_count++;
        printf("\n========== COMMAND %d ==========\n", cmd_count);

        // Afficher le chemin
        printf("Path: %s\n", current->path ? current->path : "(null)");

        // Afficher les arguments
        if (current->args)
        {
            printf("Arguments:\n");
            i = 0;
            while (current->args[i])
            {
                printf("  args[%d]: '%s'\n", i, current->args[i]);
                i++;
            }
            if (i == 0)
                printf("  (empty array)\n");
        }
        else
            printf("Arguments: (null)\n");

        // Afficher les tokens associés à cette commande
        if (current->tokens)
        {
            t_token *token = current->tokens;
            printf("Tokens:\n");
            i = 0;
            while (token)
            {
                printf("  token[%d]: type=%d, value='%s'", i, token->type,
                       token->value ? token->value : "(null)");

                if (token->is_single_quote)
                    printf(" (single quoted)");
                if (token->is_double_quote)
                    printf(" (double quoted)");

                printf("\n");
                token = token->next;
                i++;
            }
        }
        else
            printf("Tokens: (null)\n");

        // Afficher les redirections
        if (current->redir)
        {
            printf("Redirections:\n");
            redir = current->redir;
            i = 0;
            while (redir)
            {
                printf("  redir[%d]: ", i);
                switch (redir->type)
                {
                    case REDIR_IN:
                        printf("< (input)");
                        break;
                    case REDIR_OUT:
                        printf("> (output)");
                        break;
                    case APPEND:
                        printf(">> (append)");
                        break;
                    case HEREDOC:
                        printf("<< (heredoc)");
                        break;
                    default:
                        printf("unknown type %d", redir->type);
                }

                printf(", file: '%s'\n", redir->file ? redir->file : "(null)");
                redir = redir->next;
                i++;
            }
        }
        else
            printf("Redirections: (none)\n");

        // Passer à la commande suivante (pipe)
        if (current->next)
            printf("\n----------- PIPE -----------\n");

        current = current->next;
    }
    printf("\n========== END OF COMMAND ==========\n\n");
}

static char	*get_input(void)
{
	char	*input;

	input = custom_reader();
	return (input);
}

static void process_input(char *input, t_env **env)
{
	t_token *tokens;
	t_cmd *cmd;

	if (!check_input(input) || !(*input))
		return;
	tokens = tokenize(input, *env);
	cmd = parse(tokens, env);
	if (cmd)
		print_cmd_info(cmd);
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
