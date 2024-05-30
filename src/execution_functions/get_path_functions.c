/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path_functions.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stopp <stopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 10:57:40 by fkeitel           #+#    #+#             */
/*   Updated: 2024/05/30 11:50:21 by stopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*ft_getenv(t_env *env_lst, char *name)
{
	t_env	*tmp;

	tmp = env_lst;
	while (tmp)
	{
		if (ft_strncmp(tmp->name, name, ft_strlen(tmp->name)) == 0)
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

char	**get_paths(t_env **env_lst)
{
	int		i;
	char	*path;
	char	**paths;

	i = 0;
	path = ft_getenv(*env_lst, "PATH");
	if (path)
	{
		paths = ft_split(path, ':');
		if (!paths)
			return (0);
		while (paths[i])
		{
			path = paths[i];
			paths[i] = ft_strjoin(path, "/");
			free(path);
			if (!paths[i])
				return (NULL);
			i++;
		}
		return (paths);
	}
	else
		return (NULL);
}

char	*get_cmdpath(char *cmd, t_env **env_lst, t_tree *tree)
{
	int		i;
	char	*cmd_path;
	char	**paths;

	i = 0;
	paths = get_paths(env_lst);
	while (paths[i])
	{
		cmd_path = ft_strjoin(paths[i], cmd);
		if (access(cmd_path, F_OK) == 0)
		{
			if (access(cmd_path, X_OK) == 0)
				return (free_two_dimensional_array(paths), cmd_path);
			else
			{
				dup2(2, 1);
				ft_printf("%s: Permission denied\n", tree->arguments[0]);
				exit (126);
			}
		}
		free(cmd_path);
		i++;
	}
	free_two_dimensional_array(paths);
	return (NULL);
}

int	join_name_value(t_env *env_node, char **env_array, int i)
{
	char	*tmp;

	tmp = ft_strjoin(env_node->name, "=");
	if (!tmp)
		return (0);
	env_array[i] = ft_strjoin(tmp, env_node->value);
	if (!env_array[i])
		return (free(tmp), 0);
	free (tmp);
	return (1);
}

void	absolute_path(t_tree *tmp, char **env_array)
{
	DIR		*dir;

	dir = opendir(tmp->arguments[0]);
	if (!dir)
	{
		if (access(tmp->arguments[0], X_OK) == 0)
			execve(tmp->arguments[0], tmp->arguments, env_array);
		else
		{
			dup2(2, 1);
			ft_printf("%s: Permission denied\n", tmp->arguments[0]);
			exit (126);
		}
	}
	else
	{
		closedir(dir);
		dup2(2, 1);
		ft_printf("%s: is a directory\n", tmp->arguments[0]);
		exit(0);
	}
}
