/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 00:14:12 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/28 18:22:48 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_all(t_token *tokens, t_env *env)
{
	expand_heredocs(tokens, env);
	expand_exit_code(tokens);
	expand_variable(tokens, env);
	expand_tilde(tokens, env);
}
