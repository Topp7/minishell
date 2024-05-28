/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_arg_str.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stopp <stopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 10:47:36 by fkeitel           #+#    #+#             */
/*   Updated: 2024/05/27 17:05:24 by stopp            ###   ########.fr       */
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
		else if (both_quote_checker(cmd_str, i) == 1
			&& ft_strncmp((cmd_str + i), ">>", 2) == 0
			&& cmd_str[i + 2] && cmd_str[i + 2] != '>')
		{
			cmd_str = handle_append(cmd_str, tree);
			i = 0;
		}
		else if (both_quote_checker(cmd_str, i) == 1
			&& ft_strncmp((cmd_str + i), ">", 1) == 0
			&& cmd_str[i + 1] && cmd_str[i + 1] != '>')
		{
			cmd_str = handle_trunc(cmd_str, tree);
			i = 0;
		}
		else if (both_quote_checker(cmd_str, i) == 1
			&& ft_strncmp((cmd_str + i), "<", 1) == 0
			&& cmd_str[i + 1] && cmd_str[i + 1] != '<')
		{
			cmd_str = handle_infile(cmd_str, tree);
			i = 0;
		}
		i++;
	}
	return (cmd_str);
}

int	str_empty(char *cmd_str)
{
	int	i;

	i = 0;
	while (cmd_str[i])
	{
		if (cmd_str[i] != ' ' && cmd_str[i] != '\n')
			return (1);
		i++;
	}
	return (0);
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
	if (init_tree(*tree, pipes, ex_st, pipe_num++) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	while (pipes[pipe_num])
	{
		temp = (t_tree *)malloc(sizeof(t_tree));
		if (!temp)
			return (pipes_error("error malloc", temp, pipes));
		temp->parent_pipe = parent;
		if (init_tree(temp, pipes, ex_st, pipe_num++) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		ft_treeadd_back(tree, temp, &parent);
	}
	free(command_str);
	return (EXIT_SUCCESS);
}
