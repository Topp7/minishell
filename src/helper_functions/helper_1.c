/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkeitel <fkeitel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 12:09:19 by fkeitel           #+#    #+#             */
/*   Updated: 2024/05/31 18:53:41 by fkeitel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//	lst add back function modified for the tree struct
void	ft_treeadd_back(t_tree **lst, t_tree *new, t_tree **parent)
{
	t_tree	*current;

	new->parent_pipe = *parent;
	*parent = new;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	current = *lst;
	while (current->child_pipe != NULL)
		current = current->child_pipe;
	current->child_pipe = new;
}

//	function to check if string is in single or double quotes
int	quote_checker(char *arg, int j)
{
	static int	single_quote = 0;
	static int	double_quote = 0;

	if (j == 0)
	{
		single_quote = 0;
		double_quote = 0;
	}
	if (arg[j] && arg[j] == '\'' && !(double_quote))
	{
		single_quote = !(single_quote);
	}
	else if (arg[j] && arg[j] == '\"' && !(single_quote))
	{
		double_quote = !(double_quote);
	}
	if (single_quote)
		return (0);
	return (1);
}

//	function to create the variable and the replace string for the env search
int	create_var_and_rep_str(char **var, char	**replace, char *arg, int j)
{
	*var = malloc(sizeof(char) * (j + 1));
	if (!var)
		return (-1);
	(*var)[0] = '$';
	ft_strlcpy(*var + 1, arg, j);
	*(replace) = malloc(sizeof(char));
	if (!(*replace))
		return (free(*var), -1);
	(*replace)[0] = '\0';
	return (1);
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
