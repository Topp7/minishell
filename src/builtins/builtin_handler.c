/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkeitel <fkeitel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 15:15:21 by stopp             #+#    #+#             */
/*   Updated: 2024/05/12 07:49:59 by fkeitel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	handle_builtins(t_tree *tree)
{
	if (tree->command == ECHO)
		ft_echo(tree);
	if (tree->command == PWD)
		ft_pwd(tree);
}
