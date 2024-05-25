/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_arg_str.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkeitel <fkeitel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 10:47:36 by fkeitel           #+#    #+#             */
/*   Updated: 2024/05/25 15:41:26 by fkeitel          ###   ########.fr       */
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
int	adapt_and_count_arguments(t_tree *tree, char *command_str)
{
	int	i;

	i = 1;
	tree->arguments = split_pipes(command_str, ' ', &i);
	if (tree->arguments == NULL)
		return (pipes_error("error split", tree, NULL));
	tree->args_num = i;
	return (EXIT_SUCCESS);
}

int	skip_here(int *i, char *str, char *here_doc)
{
	if (ft_strncmp(&str[*i], "<<", 2) == 0)
	{
		*i += 2;
		while (str[*i] == ' ')
			*i += 1;
		*i += ft_strlen(here_doc);
		return (1);
	}
	else
		return (0);
}

char	*create_str(char *str, char *here_doc)
{
	int		i;
	int		j;
	char	*new_str;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (skip_here(&i, str, here_doc) == 0)
		{
			j++;
			i++;
		}
	}
	new_str = malloc(j + 1);
	new_str[j] = '\0';
	i = 0;
	j = 0;
	while (str[i])
	{
		if (skip_here(&i, str, here_doc) == 0)
			new_str[j++] = str[i++];
	}
	free (str);
	return (new_str);
}

char	*create_heredoc(char *str, char *cmd_str)
{
	int		fd[2];
	char	*buf;
	char	*new_cmdstr;

	buf = NULL;
	if (pipe(fd) == -1)
		return (NULL);
	while (1)
	{
		buf = get_next_line(STDIN_FILENO);
		if (buf == NULL)
			break ;
		if (ft_strncmp(buf, str, ft_strlen(buf)) == 10)
			break ;
		write(fd[1], buf, ft_strlen(buf));
		free(buf);
	}
	free(buf);
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	new_cmdstr = create_str(cmd_str, str);
	return (new_cmdstr);
}

//	function to save the heredoc input in a string
char	*handle_heredoc(char *cmd_str)
{
	char	*here_str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (cmd_str[i])
	{
		if (both_quote_checker(cmd_str, i) == 1
			&& ft_strncmp((cmd_str + i), "<<", 2) == 0
			&& cmd_str[i + 2] && cmd_str[i + 2] != '<')
		{
			i += 2;
			while (cmd_str[i] && cmd_str[i] == ' ')
				i++;
			while ((cmd_str[i + j] && cmd_str[i + j] != ' '))
				j++;
			here_str = malloc(sizeof(char) * (j + 1));
			if (!here_str)
				return (NULL);
			ft_strlcpy(here_str, cmd_str + i, j + 1);
			cmd_str = create_heredoc(here_str, cmd_str);
			i = 0;
			free(here_str);
		}
		i++;
	}
	i = 0;
	both_quote_checker(" ", i);
	return (cmd_str);
}

// char	*handle_redirects(char *cmd_str)
// {
// 	int i;
// 	int	j;

// 	i = 0;
// 	j = 0;
// 	while (cmd_str[i])
// 	{
// 		if (both_quote_checker(cmd_str, i) == 1 && ft_strncmp((cmd_str + i), "<<", 2) == 0
// 			&& cmd_str[i + 2] && cmd_str[i + 2] != '<')
// 			handle_heredoc(cmd_str);
// 		if (both_quote_checker(cmd_str, i) == 1 && ft_strncmp((cmd_str + i), ">>", 2) == 0
// 			&& cmd_str[i + 2] && cmd_str[i + 2] != '<')
// 			handle_append(cmd_str);
// 		if (both_quote_checker(cmd_str, i) == 1 && ft_strncmp((cmd_str + i), "<", 2) == 0
// 			&& cmd_str[i + 2] && cmd_str[i + 2] != '<')
// 			handle_trunc(cmd_str);
// 		if (both_quote_checker(cmd_str, i) == 1 && ft_strncmp((cmd_str + i), ">", 2) == 0
// 			&& cmd_str[i + 2] && cmd_str[i + 2] != '<')
// 			handle_outfile(cmd_str);
// 	}
// }

//	function to split the commands into the components
int	split_command(t_tree *tree, char *command_str, int ex_st)
{
	if (det_and_rem_quotes_first_word(command_str) == EXIT_FAILURE
		|| adapt_and_count_arguments(tree, command_str) == EXIT_FAILURE
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
	command_str = handle_heredoc(command_str);
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
	free_two_dimensional_array(pipes);
	free(command_str);
	return (EXIT_SUCCESS);
}
