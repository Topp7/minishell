/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_arg_str.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stopp <stopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 10:47:36 by fkeitel           #+#    #+#             */
/*   Updated: 2024/05/27 12:29:39 by stopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//	function to read the user input from the terminal command
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#define ESCAPE_SEQUENCE "\x1b"

//	count_arguments and remove quotes
int	adapt_and_count_arguments(t_tree *tree, char **command_str)
{
	int	i;

	i = 1;
	*command_str = handle_redirects(*command_str, tree);
	tree->arguments = split_pipes(*command_str, ' ', &i);
	if (tree->arguments == NULL)
		return (pipes_error("error split", tree, NULL));
	tree->args_num = i;
	return (EXIT_SUCCESS);
}

char	*handle_redirects(char *cmd_str, t_tree *tree)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (cmd_str[i])
	{
		if (both_quote_checker(cmd_str, i) == 1
			&& ft_strncmp(&cmd_str[i], "<<", 2) == 0
			&& cmd_str[i + 2] && cmd_str[i + 2] != '<')
		{
			cmd_str = handle_heredoc(cmd_str, tree);
			i = 0;
		}
		if (both_quote_checker(cmd_str, i) == 1
			&& ft_strncmp((cmd_str + i), ">>", 2) == 0
			&& cmd_str[i + 2] && cmd_str[i + 2] != '>')
		{
			cmd_str = handle_append(cmd_str, tree);
			i = 0;
		}
		// if (both_quote_checker(cmd_str, i) == 1 && ft_strncmp((cmd_str + i), "<", 2) == 0
		// 	&& cmd_str[i + 2] && cmd_str[i + 2] != '<')
		// 	handle_trunc(cmd_str);
		// if (both_quote_checker(cmd_str, i) == 1 && ft_strncmp((cmd_str + i), ">", 2) == 0
		// 	&& cmd_str[i + 2] && cmd_str[i + 2] != '<')
		// 	handle_outfile(cmd_str);
		i++;
	}
	return (cmd_str);
}

//	function to split the commands into the components
int	split_command(t_tree *tree, char *command_str, int ex_st)
{
	if (det_and_rem_quotes_first_word(command_str) == EXIT_FAILURE
		|| adapt_and_count_arguments(tree, &command_str) == EXIT_FAILURE
		|| expander(tree->arguments, tree->env, ex_st) == EXIT_FAILURE)
		return (printf("error in parsing!\n"), EXIT_FAILURE);
	if (is_substr_first_word(command_str, "echo"))
		tree->command = ECHO;
	else if (is_substr_first_word(command_str, "pwd"))
		tree->command = PWD;
	else if (is_substr_first_word(command_str, "cd"))
		tree->command = CD;
	else if (is_substr_first_word(command_str, "env"))
		tree->command = ENV;
	else if (is_substr_first_word(command_str, "unset"))
		tree->command = UNSET;
	else if (is_substr_first_word(command_str, "export"))
		tree->command = EXPORT;
	else if (is_substr_first_word(command_str, "exit"))
		tree->command = EXIT;
	tree->cmd_brch = ft_strdup(command_str);
	if (!tree->cmd_brch)
		return (pipes_error("Error in strdup\n", tree, NULL));
	return (EXIT_SUCCESS);
}

//	function to seperate the pipes into the arguments and assign everything
int	build_command_tree(t_tree **tree, char *command_str)
{
	int		pipe_num;
	char	**pipes;
	t_tree	*temp;
	t_tree	*parent;
	int		ex_st;

	parent = *tree;
	ex_st = (*tree)->exit_status;
	pipes = split_pipes(command_str, '|', &pipe_num);
	if (!pipes)
		return (pipes_error("error split", NULL, pipes));
	pipe_num = 0;
	while (pipes[pipe_num])
	{
		if (pipe_num == 0)
		{
			initiliaze_command_tree(*tree, pipe_num);
			if (split_command(*tree, pipes[pipe_num], ex_st) == EXIT_FAILURE)
				return (pipes_error("error split_command", *tree, pipes));
		}
		else
		{
			temp = (t_tree *)malloc(sizeof(t_tree));
			if (!temp)
				return (pipes_error("error malloc", temp, pipes));
			temp->parent_pipe = parent;
			initiliaze_command_tree(temp, pipe_num);
			if (split_command(temp, pipes[pipe_num], ex_st) == EXIT_FAILURE)
				return (pipes_error("error split_command", temp, pipes));
			ft_treeadd_back(tree, temp, &parent);
		}
		pipe_num++;
	}
	free(command_str);
	return (EXIT_SUCCESS);
}
