/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 17:18:27 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/25 00:55:30 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
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

/** Utils */
int		ft_is_path(char c);

/** Fonctions de netoyage */
void	free_tokens(t_token	*tokens);
void	free_env(t_env *env);
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

/** Expand */
char	*get_var_name(char *value);
char	*replace_var(char *str, char *var_name, t_env *env);
char	*add_char_to_str(char *str, char c);
void	expand_variable(t_token *tokens, t_env *env);
void	expand_tilde(t_token *tokens, t_env *env);
void	expand_all(t_token *tokens, t_env *env);

//buitlin.c
int		check_builtin(t_cmd *cmd);
void	exec_builtin(t_cmd *cmd, char ***env);

// env.c
t_env	*env_new_var(char *name, char *value, int equal_sign);
void	env_add_back(t_env **env, t_env *new);
void	env_init(t_env **env_list, char **env);
char	*get_env_value(t_env *env_list, char *name);
void	set_env_value(t_env *env_list, char *name, char *value);
void	unset_env_var(t_env **env_list, char *name);
char	*get_name(char *str);
char	*get_value(char *str);

#endif
