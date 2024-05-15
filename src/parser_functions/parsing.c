/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkeitel <fkeitel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 10:47:36 by fkeitel           #+#    #+#             */
/*   Updated: 2024/05/15 17:41:43 by fkeitel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//	initialize the command_tree struct
void	initiliaze_command_tree(t_tree *tree, int i, t_env **env_lst)
{
	tree->type = 0;
	tree->command = 0;
	tree->flags = NULL;
	tree->arguments = NULL;
	tree->cmd_brch = NULL;
	tree->child_pipe = NULL;
	tree->pipes_num = 0;
	tree->output = 0;
	tree->args_num = 0;
	tree->pipes_num = i + 1;
	tree->env = env_lst;
}

//	function to parse the argument into parsing struct for cmd_exec funciton
t_tree	*parse_command(char **command_str, t_env **env_lst)
{
	t_tree	*tree;

	tree = NULL;
	add_history(*command_str);
	if (*command_str)
	{
		if (check_for_quotes_and_slash(*command_str) == EXIT_FAILURE)
		{
			free_env_list(env_lst);
			free(*command_str);
			clear_history();
			return (NULL);
		}
		if (build_command_tree(&tree, *command_str, env_lst) == EXIT_FAILURE)
		{
			free_env_list(env_lst);
			free(*command_str);
			clear_history();
			return (NULL);
		}
	}
	free(*command_str);
	return (tree);
}
