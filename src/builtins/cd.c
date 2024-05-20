/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stopp <stopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 16:34:22 by stopp             #+#    #+#             */
/*   Updated: 2024/05/20 18:04:51 by stopp            ###   ########.fr       */
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

void	change_to_parent(t_tree *tree)
{
	export(tree, ft_strjoin("OLDPWD=", getcwd(NULL, 0)));
	chdir("..");
	export(tree, ft_strjoin("PWD=", getcwd(NULL, 0)));
	return ;
}

void	change_to_previous(t_tree *tree)
{
	t_env	*oldwd;

	oldwd = *tree->env;
	while (oldwd->next)
	{
		if (ft_strncmp(oldwd->name, "OLDPWD", 7) == 0)
			break ;
		oldwd = oldwd->next;
	}
	if (!oldwd)
		ft_printf("OLDPWD not set\n");
	else
	{
		export(tree, ft_strjoin("OLDPWD=", ft_getenv(*tree->env, "PWD")));
		chdir(oldwd->value);
		export(tree, ft_strjoin("PWD=", oldwd->value));
	}
}

void	change_to_home(t_tree *tree)
{
	char	*home_path;

	home_path = getenv("HOME");
	ft_printf("%s\n", home_path);
	export(tree, ft_strjoin("OLDPWD=", getcwd(NULL, 0)));
	chdir(home_path);
	export(tree, ft_strjoin("PWD=", home_path));
}

void	ft_chdir(t_tree *tree, t_env **env_lst)
{
	(void)env_lst;
	if (tree->arguments[2] != NULL)
		ft_printf("cd: string not in pwd: %s\n", tree->arguments[1]);
	if (ft_strncmp(tree->arguments[1], "..", 3) == 0)
		change_to_parent(tree);
	else if (ft_strncmp(tree->arguments[1], "-", 2) == 0)
		change_to_previous(tree);
	else if (ft_strncmp(tree->arguments[1], "~", 2) == 0)
		change_to_home(tree);
	// else if (ft_strncmp(tree->arguments[1], ".", 2) == 0)
	// 	return ;
	// else
	// 	change_dir;
}
