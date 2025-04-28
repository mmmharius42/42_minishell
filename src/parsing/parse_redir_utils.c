/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 15:30:00 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/28 18:36:16 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Ajoute une redirection à la fin de la liste de redirections.
** Si la liste est vide, initialise la liste avec la redirection.
*/
void	add_redirection(t_redir **redir_list, t_redir *redir)
{
	t_redir	*current;

	if (!redir_list || !redir)
		return ;
	if (*redir_list == NULL)
	{
		*redir_list = redir;
		return ;
	}
	current = *redir_list;
	while (current->next)
		current = current->next;
	current->next = redir;
}

/*
** Affiche une erreur de redirection.
** Retourne toujours 0 pour indiquer une erreur.
*/
int	redirect_error(char *file)
{
	if (!file)
	{
		ft_putstr_fd("minishell: heredoc interrompu\n", 2);
		return (0);
	}
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(file, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
	g_return_code = 1;
	return (0);
}
