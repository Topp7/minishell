/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stopp <stopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 16:34:22 by stopp             #+#    #+#             */
/*   Updated: 2024/05/23 15:08:59 by stopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	change_to_parent(t_tree *tree)
{
	export(tree, ft_strjoin("OLDPWD=", ft_getenv(*tree->env, "PWD")));
	chdir("..");
	export(tree, ft_strjoin("PWD=", getcwd(NULL, 0)));
	return ;
}

void	change_to_previous(t_tree *tree)
{
	t_env	*oldwd;

	oldwd = *tree->env;
	while (oldwd)
	{
		if (ft_strncmp(oldwd->name, "OLDPWD", 7) == 0)
			break ;
		oldwd = oldwd->next;
	}
	if (!oldwd)
		ft_printf("OLDPWD not set\n");
	else
	{
		printf("%s\n", oldwd->value);
		chdir(oldwd->value);
		export(tree, ft_strjoin("OLDPWD=", ft_getenv(*tree->env, "PWD")));
		export(tree, ft_strjoin("PWD=", getcwd(NULL, 0)));
	}
}

void	change_to_home(t_tree *tree, char c)
{
	char	*home_path;

	if (c == '~')
		home_path = getenv("HOME");
	else
	{
		home_path = ft_getenv(*tree->env, "HOME");
		if (!home_path)
		{
			ft_printf("cd: HOME not set\n");
			return ;
		}
	}
	export(tree, ft_strjoin("OLDPWD=", getcwd(NULL, 0)));
	chdir(home_path);
	export(tree, ft_strjoin("PWD=", home_path));
}

int	check_dir(t_tree *tree, char *dir)
{
	struct stat	*buf;

	buf = malloc(sizeof(struct stat));
	if (!buf)
		return (0);
	if (stat(dir, buf) == -1)
	{
		ft_printf("cd: %s: No such file or directory\n", tree->arguments[1]);
		return (0);
	}
	else if (!S_ISDIR(buf->st_mode))
	{
		ft_printf("cd: %s: Not a directory\n", tree->arguments[1]);
		return (0);
	}
	return (1);
}

void	change_dir(t_tree *tree)
{
	char	*new_dir;

	if (tree->arguments[1] == ft_strchr(tree->arguments[1], '/'))
	{
		if(!check_dir(tree, tree->arguments[1]))
			return ;
		export(tree, ft_strjoin("OLDPWD=", getcwd(NULL, 0)));
		chdir(tree->arguments[1]);
		export(tree, ft_strjoin("PWD=", getcwd(NULL, 0)));
	}
	else
	{
		new_dir = ft_strjoin(getcwd(NULL, 0), "/");
		free(new_dir);
		new_dir = ft_strjoin(new_dir, tree->arguments[1]);
		if (!check_dir(tree, new_dir))
			return (free(new_dir));
		export(tree, ft_strjoin("OLDPWD=", getcwd(NULL, 0)));
		chdir(new_dir);
		export(tree, ft_strjoin("PWD=", getcwd(NULL, 0)));
		free(new_dir);
	}
}

void	ft_chdir(t_tree *tree, t_env **env_lst)
{
	(void)env_lst;
	if (!tree->arguments[1])
		change_to_home(tree, 0);
	else if (ft_strncmp(tree->arguments[1], "..", 3) == 0)
		change_to_parent(tree);
	else if (ft_strncmp(tree->arguments[1], "-", 2) == 0)
		change_to_previous(tree);
	else if (ft_strncmp(tree->arguments[1], "~", 2) == 0)
		change_to_home(tree, '~');
	else if (ft_strncmp(tree->arguments[1], ".", 2) == 0)
		return ;
	else if (tree->arguments[2] != NULL)
		ft_printf("cd: string not in pwd: %s\n", tree->arguments[1]);
	else
		change_dir(tree);
}
