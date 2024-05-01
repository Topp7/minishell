/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stopp <stopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 14:45:53 by stopp             #+#    #+#             */
/*   Updated: 2024/04/30 15:25:11 by stopp            ###   ########.fr       */
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
		printf("\n");
		exit (0);
	}
	if (ft_strncmp(tree->arguments[i], "-n", 2) == 0)
	{
		n_chk = 1;
		i = 2;
	}
	while (tree->arguments[i])
		printf("%s", tree->arguments[i++]);
	if (!n_chk)
		printf("\n");
	exit (0);
}
