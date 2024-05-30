/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stopp <stopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 16:34:22 by stopp             #+#    #+#             */
/*   Updated: 2024/05/30 11:29:33 by stopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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
		export(tree, strjoin_free("PWD=", getcwd(NULL, 0), 2));
	}
}

void	change_to_home(t_tree *tree)
{
	char	*home_path;

	home_path = ft_getenv(*tree->env, "HOME");
	if (!home_path)
	{
		ft_printf("cd: HOME not set\n");
		return ;
	}
	export(tree, strjoin_free("OLDPWD=", getcwd(NULL, 0), 2));
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
		dup2(2, 1);
		ft_printf("cd: %s: No such file or directory\n", tree->arguments[1]);
		dup2(1, 1);
		free(buf);
		return (0);
	}
	else if (!S_ISDIR(buf->st_mode))
	{
		dup2(2, 1);
		ft_printf("cd: %s: Not a directory\n", tree->arguments[1]);
		dup2(1, 1);
		free(buf);
		return (0);
	}
	free(buf);
	return (1);
}

void	change_dir(t_tree *tree)
{
	char	*new_dir;

	if (tree->arguments[1] == ft_strchr(tree->arguments[1], '/'))
	{
		if (!check_dir(tree, tree->arguments[1]))
		{
			tree->exit_status = 1;
			tree->signal_exit = 1;
			return ;
		}
		export(tree, strjoin_free("OLDPWD=", getcwd(NULL, 0), 2));
		chdir(tree->arguments[1]);
		export(tree, strjoin_free("PWD=", getcwd(NULL, 0), 2));
	}
	else
	{
		new_dir = strjoin_free(getcwd(NULL, 0), "/", 1);
		new_dir = strjoin_free(new_dir, tree->arguments[1], 1);
		if (!check_dir(tree, new_dir))
			return (free(new_dir));
		export(tree, strjoin_free("OLDPWD=", getcwd(NULL, 0), 2));
		chdir(new_dir);
		export(tree, strjoin_free("PWD=", getcwd(NULL, 0), 2));
		free(new_dir);
	}
}

void	ft_chdir(t_tree *tree, t_env **env_lst)
{
	(void)env_lst;
	if (!tree->arguments[1])
		change_to_home(tree);
	else if (ft_strncmp(tree->arguments[1], "..", 3) == 0)
	{
		export(tree, strjoin_free("OLDPWD=", getcwd(NULL, 0), 2));
		chdir("..");
		export(tree, strjoin_free("PWD=", getcwd(NULL, 0), 2));
	}
	else if (ft_strncmp(tree->arguments[1], "-", 2) == 0)
		change_to_previous(tree);
	else if (ft_strncmp(tree->arguments[1], ".", 2) == 0)
		return ;
	else
		change_dir(tree);
}
