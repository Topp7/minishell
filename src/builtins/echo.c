/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stopp <stopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 14:45:53 by stopp             #+#    #+#             */
/*   Updated: 2024/05/29 13:23:52 by stopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_echo(t_tree *tree)
{
	int	i;
	int	n_chk;

	i = 1;
	n_chk = 0;
	if (!tree->arguments[i])
	{
		ft_printf("\n");
		return ;
	}
	while (ft_strncmp(tree->arguments[i], "-n", 2) == 0)
	{
		n_chk = 1;
		i++;
	}
	while (tree->arguments[i])
	{
		ft_printf("%s", tree->arguments[i]);
		if (tree->arguments[++i])
			ft_printf(" ");
	}
	if (!n_chk)
		ft_printf("\n");
	return ;
}
