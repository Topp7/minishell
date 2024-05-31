/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkeitel <fkeitel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 15:15:21 by stopp             #+#    #+#             */
/*   Updated: 2024/05/30 20:13:28 by fkeitel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	update_exit(t_tree *tree, int exec_exit)
{
	if (WIFEXITED(tree->exit_status))
		tree->exit_status = WEXITSTATUS(tree->exit_status);
	if (tree->exit_status > 1 && exec_exit != 0)
		tree->exit_status += 128;
}

int	exit_handler(t_tree *tree)
{
	int	i;

	i = 0;
	if (tree->pipes_num == 1)
		printf("exit\n");
	if (tree->arguments[1])
	{
		while (tree->arguments[1][i])
		{
			if (*tree->arguments[1] != 45 && (!ft_isdigit(tree->arguments[1][i])
				&& tree->arguments[1][i] != '+'))
			{
				return (print_str_return_exit
					("numeric argument required\n", 255));
			}
			i++;
		}
		if (tree->arguments[2])
		{
			tree->command = 0;
			return (print_str_return_exit("exit: too many arguments\n", 1));
		}
		tree->exit_status = ft_atoi(tree->arguments[1]) % 256;
	}
	return (tree->exit_status);
}

void	execute_builtin(t_tree *tree, t_env **env_lst)
{
	char	*dup_ex;

	if (tree->command == ECHO)
		ft_echo(tree);
	if (tree->command == PWD)
		ft_pwd();
	if (tree->command == CD)
		ft_chdir(tree, env_lst);
	if (tree->command == ENV)
		print_env(tree);
	if (tree->command == UNSET)
		ft_unset(tree, tree->arguments[1]);
	if (tree->command == EXPORT)
	{
		dup_ex = ft_strdup(tree->arguments[1]);
		export(tree, dup_ex);
	}
	if (tree->command == EXIT)
	{
		tree->exit_status = exit_handler(tree);
		if (tree->command == EXIT && !tree->child_pipe && !tree->parent_pipe)
			tree->signal_exit = 1;
	}
}

void	handle_builtins(t_tree *tree, t_env **env_lst)
{
	int		fd[2];
	int		pipebool;

	pipebool = 0;
	wait(NULL);
	if (tree->child_pipe)
	{
		if (pipe(fd) == -1)
			return ;
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		pipebool = 1;
	}
	open_close_fds(tree);
	execute_builtin(tree, env_lst);
	if (pipebool == 1)
	{
		dup2(fd[0], STDIN_FILENO);
		close (fd[0]);
	}
	dup2(tree->stdoutput, STDOUT_FILENO);
}
