/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stopp <stopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 16:34:22 by stopp             #+#    #+#             */
/*   Updated: 2024/05/01 12:19:04 by stopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	cd_previous(t_tree *tree, char *previous_dir, char *current_dir)
{
	if (pervious_dir)
	previous_dir = ft_calloc(4096, 1);
		ft_strncpy(previous_dir, current_dir, ft_strlen(current_wd));
}

void	ft_chdir(t_tree *tree, char c)
{
	static char	*previous_dir;
	char		*current_dir;

	if (previous_dir)
		free(previous_dir);
	previous_dir = NULL;
	if (c != 'f')
	{
		current_dir = getcwd(NULL, 0);
		if (tree->arguments[1][0] == '-')
			cd_previous();
		previous_dir = ft_calloc(4096, 1);
		ft_strncpy(previous_dir, current_dir, ft_strlen(current_wd));

	}
}
