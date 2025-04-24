/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 17:18:27 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/24 23:47:22 by mpapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <signal.h>
# include <sys/stat.h>
# include "libft.h"
# include "get_next_line.h"

# define MAX_LINE 1024

# define RED "\033[0;31m"
# define RESET "\033[0m"

# define WORD 0
# define PIPE 1
# define REDIR_IN 2
# define REDIR_OUT 3
# define APPEND 4
# define HEREDOC 5

typedef struct s_redir
{
	char			*file;
	int				type;
	struct s_redir	*next;
}	t_redir;

typedef struct s_env
{
	char			*name;
	char			*value;
	int				equal_sign;
	struct s_env	*next;
}	t_env;

typedef struct s_token
{
	char			*value;
	int				type;
	int				is_single_quote;
	int				is_double_quote;
	struct s_token	*next;
}	t_token;

typedef struct s_cmd
{
	char	*path;
	char	**args;
}	t_cmd;

/** Fonctions de netoyage */
void	free_tokens(t_token	*tokens);
void	clean_shell(char *input, t_token *tokens);

/** Input */
char	*custom_reader(void);
int		check_input(char *input);

/** Token */
t_token	*tokenize(char *input);
t_token	*create_token(char *value, int type, int is_single_quote,
			int is_double_quote);
t_token	*process_token(char *input, int *i);
void	add_token(t_token **tokens, t_token *new_token);
char	*extract_word(char *input, int *i, int *is_single_quote,
			int *is_double_quote);
char	*extract_operator(char *input, int *i);
int		get_token_type(char *str);
int		is_special_char(char c);
void	skip_spaces(char *input, int *i);
void	print_tokens(t_token *tokens);
int		token_count(t_token *tokens);

/** Builtin */
// builtin.c
int		check_builtin(t_cmd *cmd);
void	exec_builtin(t_cmd *cmd, char ***env);

// utils/utils.c
char	*get_name(char *str);
char	*get_value(char *str);

// env.c
void	ft_env(t_env *env_list);

#endif
