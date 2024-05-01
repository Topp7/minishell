/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stopp <stopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 15:32:06 by stopp             #+#    #+#             */
/*   Updated: 2024/04/30 16:06:48 by stopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_pwd(t_tree *tree)
{
	char	*buf;

	buf = getcwd(NULL, 0);
	printf("%s", buf);
	if (!tree->child_pipe)
		printf("\n");
	free(buf);
	// printf("test\n");
	exit (0);
}
