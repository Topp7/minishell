/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_arg_str.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkeitel <fkeitel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 10:47:36 by fkeitel           #+#    #+#             */
/*   Updated: 2024/05/29 18:39:30 by fkeitel          ###   ########.fr       */
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
int	adapt_and_count_arguments(t_tree *tree, char **command_str, int *ex_st)
{
	int	i;

	i = 1;
	(void)ex_st;
	tree->arguments = split_with_quotes(command_str, ' ', &i);
	if (tree->arguments == NULL)
		return (pipes_error("error split", tree, NULL));
	tree->args_num = i;
	return (EXIT_SUCCESS);
}

typedef char	*(*t_cmd_func)(char *cmd_str, t_tree *tree);

char	*exec_cmd_funct(char **cmd_str, t_tree *tree, int *i, t_cmd_func func)
{
	*cmd_str = func(*cmd_str, tree);
	*i = 0;
	return (*cmd_str);
}

char	**cpy_args(char **new, char **args)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (args[i])
	{
		if (args[i][0] != '\0')
			new[j++] = ft_strdup(args[i]);
		i++;
	}
	free_two_dimensional_array(args);
	return (new);
}

char	**update_args(char **args)
{
	char	**new;
	int		count;
	int		i;

	count = 0;
	i = 0;
	new = args;
	while (new[i])
	{
		if (new[i] && new[i][0] != '\0')
			count++;
		i++;
	}
	new = malloc(sizeof(char *) * count + 1);
	if (!new)
		return (NULL);
	new[count] = NULL;
	cpy_args(new, args);
	return (new);
}

int	prep_heredoc(char ***args1, int i, int j, t_tree *tree)
{
	char	*to_free;
	char	**args;

	to_free = NULL;
	args = *args1;
	if (args[i][j + 2])
		args[i] = handle_heredoc(args[i], tree);
	else
	{
		to_free = args[i];
		args[i] = ft_strjoin(args[i], args[i + 1]);
		args[i + 1][0] = '\0';
		free(to_free);
		args[i] = handle_heredoc(args[i], tree);
	}
	*args1 = update_args(args);
	return (0);
}

//	function to handle all redirecton cases from the command string
char	**handle_redirects(char **args, t_tree *tree)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (args[i])
	{
		j = 0;
		while (args[i][j])
		{
			if (ft_strncmp(&args[i][j], "<<", 2) == 0
				&& args[i][j + 2] != '<' && (args[i][j + 2] || args[i + 1]))
				i = prep_heredoc(&args, i, j, tree);
			// else if (ft_strncmp((cmd_str + i), ">>", 2) == 0
			// 	&& cmd_str[i + 2] && cmd_str[i + 2] != '>')
			// 	exec_cmd_funct(&cmd_str, tree, &i, handle_append);
			// else if (ft_strncmp((cmd_str + i), ">", 1) == 0
			// 	&& cmd_str[i + 1] && cmd_str[i + 1] != '>')
			// 	exec_cmd_funct(&cmd_str, tree, &i, handle_trunc);
			// else if (ft_strncmp((cmd_str + i), "<", 1) == 0
			// 	&& cmd_str[i + 1] && cmd_str[i + 1] != '<')
			// 	exec_cmd_funct(&cmd_str, tree, &i, handle_infile);
			j++;
		}
		if (args[i])
			i++;
	}
	args = update_args(args);
	return(args);
}

//	function to split the commands into the components
int	split_command(t_tree *tree, char **command_str, int ex_st)
{
	if (null_term_string(command_str) == EXIT_FAILURE
		|| det_and_rem_quotes_first_word(*command_str) == EXIT_FAILURE
		|| adapt_and_count_arguments(tree, command_str, &ex_st) == EXIT_FAILURE
		|| expander(tree->arguments, tree->env, ex_st, tree) == EXIT_FAILURE)
		return (printf("error in parsing!\n"), EXIT_FAILURE);
	if (is_substr_first_word(*command_str, "echo"))
		tree->command = ECHO;
	else if (is_substr_first_word(*command_str, "pwd"))
		tree->command = PWD;
	else if (is_substr_first_word(*command_str, "cd"))
		tree->command = CD;
	else if (is_substr_first_word(*command_str, "env"))
		tree->command = ENV;
	else if (is_substr_first_word(*command_str, "unset"))
		tree->command = UNSET;
	else if (is_substr_first_word(*command_str, "export"))
		tree->command = EXPORT;
	else if (is_substr_first_word(*command_str, "exit"))
		tree->command = EXIT;
	tree->cmd_brch = ft_strdup(*command_str);
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
	pipes = split_with_quotes(&command_str, '|', &pipe_num);
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
	return (free(command_str), free_two_dimensional_array(pipes), EXIT_SUCCESS);
}
