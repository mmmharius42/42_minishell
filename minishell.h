/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 17:18:27 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/25 01:35:36 by aberenge         ###   ########.fr       */
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
	char			*path;
	char			**args;
	t_redir			*redir;
	t_token			*tokens;
	struct s_cmd	*next;
}	t_cmd;

extern int	g_return_code;

/** Utils */
int		ft_is_path(char c);

/** Fonctions de netoyage */
void	free_tokens(t_token	*tokens);
void	free_env(t_env *env);
void	clean_shell(char *input, t_token *tokens, t_cmd *cmd);
void	*free_commands(t_cmd *cmd_list);
void	free_command(t_cmd *cmd);
void	free_redirections(t_redir *redir);

/** Input */
char	*custom_reader(void);
int		check_input(char *input);

/** Token */
t_token	*tokenize(char *input, t_env *env);
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
int		token_count(t_token *tokens);

/** Expand */
char	*get_var_name(char *value);
char	*replace_var(char *str, char *var_name, t_env *env);
char	*add_char_to_str(char *str, char c);
void	expand_variable(t_token *tokens, t_env *env);
void	expand_tilde(t_token *tokens, t_env *env);
void	expand_all(t_token *tokens, t_env *env);

/** Parsing */
t_cmd	*parse_tokens(t_token *tokens);
int		process_token_to_cmd(t_token *token, t_cmd **current_cmd,
		t_cmd **cmd_list);
int		is_redirection(int type);
void	add_token_to_cmd(t_token *token, t_cmd *cmd);
int		parse_error(char *message);

t_cmd	*create_command(void);
void	add_command(t_cmd **cmd_list, t_cmd *cmd);
int		process_redirection(t_token *token, t_cmd **cmd);
t_redir	*create_redirection(int type, char *file);
void	add_redirection(t_redir **redir_list, t_redir *redir);
int		apply_redirections(t_redir *redir);
int		redirect_error(char *file);

int		count_args(t_token *tokens);
char	**fill_args(t_token *tokens, int count);
int		prepare_args(t_cmd *cmd);
int		prepare_all_args(t_cmd *cmd_list);
int		resolve_paths(t_cmd *cmd_list, t_env *env);

int		is_executable(char *path);
char	*create_path(char *dir, char *file);
char	*find_executable(char *cmd, char *path_env);
int		is_builtin(char *cmd);
int		finalize_commands(t_cmd *cmd_list, t_env *env);

t_cmd	*parse(t_token *tokens, t_env **env);

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
