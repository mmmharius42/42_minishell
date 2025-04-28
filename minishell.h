/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 17:18:27 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/28 13:04:27 by aberenge         ###   ########.fr       */
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
void	expand_exit_code(t_token *tokens);
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
int		apply_redirections(t_redir *redir);

void	add_redirection(t_redir **redir_list, t_redir *redir);
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
// void	exec_builtin(t_cmd *cmd, char ***env);
void	exec_builtin(t_cmd *cmd, t_env **env);

// pwd.c
void	ft_pwd(void);

//  unset.c
void    ft_unset(t_cmd *cmd, t_env **env);

// exit.c
void	ft_exit(t_cmd *cmd, t_env **env);


// cd.c
void	ft_cd(t_cmd *cmd, t_env **env);
char	*get_pwd_value(void);
void	export_variable(const char *name, const char *value, t_env **env);
char	*get_target_path(t_cmd *cmd, t_env *env);
void	update_pwd_env(t_env **env, char *old_pwd);

// echo.c
void	ft_echo(char **args);

// env.c
void	ft_env(t_env *env_list);
t_env	*env_new_var(char *name, char *value, int equal_sign);
void	env_add_back(t_env **env, t_env *new);
void	env_init(t_env **env_list, char **env);
char	*get_env_value(t_env *env_list, char *name);
void	unset_env_var(t_env **env_list, char *name);
int		var_exists(t_env *env, char *var);
void	update_var(t_env *env, char *name, char *value, int equal);
char	*get_name(char *str);
char	*get_value(char *str);
void	sort_env(t_env **var, int size);
int		env_len(t_env *env);
char	*clean_quotes(char *value);
t_env	*create_env_node(const char *name, const char *value);

// export.c
void	ft_export(char **args, t_env **env);

//history.c
char	*get_history_file_path(void);
void	load_history(void);
void	save_history(char *input);
void	ft_history(void);

// Execution
void	exec(t_cmd *cmd, t_env **env);
void	execute_simple_command(t_cmd *cmd, t_env **env);
void	execute_piped_commands(t_cmd *cmd_list, t_env *env);
void	execute_command(t_cmd *cmd, t_env *env);
void	handle_builtin(t_cmd *cmd, t_env **env);
void	handle_fork_error(void);
void	wait_for_child(pid_t pid);
void	handle_redir_only(t_cmd *cmd);
void	handle_input_pipe(int prev_pipe);
void	handle_output_pipe(int *pipe_fd);
void	execute_piped_child(t_cmd *cmd, t_env *env, int prev_pipe, int *pipe_fd);
void	update_pipe_status(int *prev_pipe, int *pipe_fd, t_cmd *current);
void	wait_for_all_children(t_cmd *cmd_list);
char	**env_to_array(t_env *env);
char	**fill_env_array(t_env *env, char **env_array);
void	free_child(t_cmd *cmd, t_env *env);

// Signals
void	setup_signals_interactive(void);
void	setup_signals_heredoc(void);
void	setup_signals_child(void);
void	setup_signals_parent(void);

#endif
