/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stopp <stopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 16:32:56 by stopp             #+#    #+#             */
/*   Updated: 2024/06/06 12:31:24 by stopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_unset(t_tree *tree, char	*env)
{
	t_env	*temp;
	t_env	*un_env;

	temp = *tree->env;
	un_env = NULL;
	if (name_check(env, tree) == 0)
		return ;
	while (temp->next)
	{
		if (ft_strncmp(temp->next->name, env, ft_strlen(env)) == 0)
		{
			un_env = temp->next;
			temp->next = un_env->next;
			free(un_env->name);
			free(un_env->value);
			free(un_env);
			return ;
		}
		temp = temp->next;
	}
	tree->exit_status = 0;
}

void	unset_loop(t_tree *tree)
{
	int	i;

	i = 1;
	if (!tree->args[i])
	{
		tree->exit_status = 0;
		return ;
	}
	while (tree->args[i])
		ft_unset(tree, tree->args[i++]);
}
