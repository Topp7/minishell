/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stopp <stopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 15:15:21 by stopp             #+#    #+#             */
/*   Updated: 2024/05/31 17:16:59 by stopp            ###   ########.fr       */
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
	open_close_fds(tree);
	if (tree->command == ECHO)
		ft_echo(tree);
	else if (tree->command == PWD)
		ft_pwd();
	else if (tree->command == CD)
		ft_chdir(tree, env_lst);
	else if (tree->command == ENV)
		print_env(tree);
	else if (tree->command == UNSET)
		ft_unset(tree, tree->arguments[1]);
	else if (tree->command == EXPORT)
	{
		if(!tree->arguments[1])
			export_env(tree);
		else
			export(tree, ft_strdup(tree->arguments[1]));
	}
	else if (tree->command == EXIT)
	{
		tree->exit_status = exit_handler(tree);
		if (tree->command == EXIT && !tree->child_pipe && !tree->parent_pipe)
			tree->signal_exit = 1;
	}
}

pid_t	exec_pipe(t_tree *tree, t_env **env_lst, int *exec_exit)
{
	pid_t		pid;
	int			fd[2];

	if (pipe(fd) == -1)
		return (0);
	pid = fork();
	if (pid == -1)
		return (0);
	if (pid == 0)
	{
		close(fd[0]);
		if (tree->child_pipe)
			dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		*exec_exit = 1;
		execute_builtin(tree, env_lst);
		exit (0);
	}
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close (fd[0]);
	}
	return (pid);
}

pid_t	handle_builtins(t_tree *tree, t_env **env_lst, int *exec_exit)
{
	pid_t	pid;

	pid = 0;
	if (tree->child_pipe || tree->parent_pipe)
		pid = exec_pipe(tree, env_lst, exec_exit);
	else
		execute_builtin(tree, env_lst);
	return (pid);
}
