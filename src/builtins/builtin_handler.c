/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stopp <stopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 15:15:21 by stopp             #+#    #+#             */
/*   Updated: 2024/05/14 19:00:00 by stopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	handle_builtins(t_tree *tree, t_env **env_lst)
{
	(void)env_lst;
	if (tree->command == ECHO)
		ft_echo(tree);
	if (tree->command == PWD)
		ft_pwd(tree);
}
