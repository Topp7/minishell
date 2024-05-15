/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stopp <stopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 16:34:22 by stopp             #+#    #+#             */
/*   Updated: 2024/05/14 19:00:10 by stopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// void	cd_previous(t_tree *tree, char *previous_dir, char *current_dir)
// {
// 	if (previous_dir)
// 	{
// 	previous_dir = ft_calloc(4096, 1);
// 		ft_strncpy(previous_dir, current_dir, ft_strlen(current_dir));
// 	}
// }

// void	ft_chdir(t_tree *tree, t_env **env_lst)
// {
// 	t_env	*tmp;

// 	tmp = *env_lst;
// 	if (tree->arguments[2] != NULL)
// 		printf("cd: string not in pwd: %s\n", tree->arguments[1]);
// 	if (ft_strncmp(tree->arguments[1], "..", 3) == 0)
// 		change_to_parent(tmp);
// 	else if (ft_strncmp(tree->arguments[1], "-", 2) == 0)
// 		change_to_previous(tmp);
// 	else if (ft_strncmp(tree->arguments[1], "~", 2) == 0)
// 		change_to_home(tmp);
// 	else if (ft_strncmp(tree->arguments[1], ".", 2) == 0)
// 		return ;
// 	else
// 		change_dir;
// }
