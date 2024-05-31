/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkeitel <fkeitel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 10:47:36 by fkeitel           #+#    #+#             */
/*   Updated: 2024/05/31 14:36:43 by fkeitel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//	initialize the command_tree struct
int	init_tree(t_tree *tree, char **pipes, int ex_st, int i)
{
	tree->type = 0;
	tree->command = 0;
	tree->here_doc = NULL;
	tree->arguments = NULL;
	tree->cmd_brch = NULL;
	tree->child_pipe = NULL;
	tree->pipes_num = 0;
	tree->output = 0;
	tree->args_num = 0;
	tree->in_fd = 0;
	tree->out_fd = 0;
	tree->pipes_num = i + 1;
	tree->arrow_quote = NULL;
	if (tree->parent_pipe)
	{
		tree->exit_status = ex_st;
		tree->env = tree->parent_pipe->env;
		tree->stdinput = tree->parent_pipe->stdinput;
		tree->stdoutput = tree->parent_pipe->stdoutput;
	}
	if (split_command(tree, &pipes[i], ex_st) == EXIT_FAILURE)
		return (pipes_error("error split_command", NULL, pipes));
	tree->arguments = handle_redirects(tree->arguments, tree);
	return (EXIT_SUCCESS);
}

//	function to parse the argument into parsing struct for cmd_exec funciton
int	parse_command(char **command_str, t_tree **tree)
{
	add_history(*command_str);
	if (*command_str)
	{
		if (check_for_quotes_and_slash(*command_str) == EXIT_FAILURE)
		{
			free(*command_str);
			return (EXIT_FAILURE);
		}
		if (build_command_tree(tree, *command_str) == EXIT_FAILURE)
		{
			free(*command_str);
			return (EXIT_FAILURE);
		}
	}
	return (EXIT_SUCCESS);
}
