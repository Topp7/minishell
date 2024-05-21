/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stopp <stopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 17:06:36 by stopp             #+#    #+#             */
/*   Updated: 2024/05/21 17:21:28 by stopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	export(t_tree *tree, char *new_env)
{
	t_env	*tmp;
	t_env	*new;

	if (!new_env)
		return ;
	tmp = *(tree->env);
	new = init_node(new_env);
	if (!new)
		return ;
	free(new_env);
	while (tmp)
	{
		if (ft_strncmp(tmp->name, new->name, ft_strlen(new->name)) == 0)
		{
			free(tmp->value);
			tmp->value = new->value;
			free(new->name);
			free(new);
			return ;
		}
		tmp = tmp->next;
	}
	lstadd_back_env(tree->env, new);
}
