/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkeitel <fkeitel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 15:15:21 by stopp             #+#    #+#             */
/*   Updated: 2024/05/25 16:52:01 by fkeitel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	exit_handler(t_tree *tree)
{
	int	i;

	i = 0;
	tree->command = EXIT;
	if (tree->pipes_num == 1)
		printf("exit\n");
	if (tree->arguments[1])
	{
		while (tree->arguments[1][i])
		{
			if (tree->arguments[1][0] != '-'
				&& !ft_isdigit(tree->arguments[1][i]))
			{
				return (printf("exit: %s: numeric argument required\n", tree->arguments[1]), 255);
			}
			i++;
		}
		if (tree->arguments[2])
		{
			tree->command = 0;
			printf("bash: exit: too many arguments\n");
			return (1);
		}
		tree->exit_status = ft_atoi(tree->arguments[1]) % 256;
	}
	return (tree->exit_status);
}

void	handle_builtins(t_tree *tree, t_env **env_lst)
{
	int		fd[2];
	int		booli;
	char	*dup_ex;

	booli = 0;
	if (tree->child_pipe)
	{
		if (pipe(fd) == -1)
			return ;
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		booli = 1;
	}
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
	if (booli)
	{
		dup2(fd[0], STDIN_FILENO);
		close (fd[0]);
	}
}
