/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stopp <stopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 11:03:04 by fkeitel           #+#    #+#             */
/*   Updated: 2024/05/22 19:39:20 by stopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	prompt_loop(t_tree	**parse_tree)
{
	int		debug_mode;
	char	*command;

	debug_mode = 0;
	command = NULL;
	add_history("fjvegfuy ");
	while (!(*parse_tree)->signal_exit)
	{
		command = readline("\033[32mminishell> \033[0m");
		if (command == NULL || (*parse_tree)->signal_exit)
		{
			if (command)
				free(command);
			return ((*parse_tree)->exit_status);
		}
		if (command[0] == '\0')
		{
			free(command);
			continue ;
		}
		if (strcmp(command, "deb") == 0)
		{
			debug_mode = !debug_mode;
			free(command);
			continue ;
		}
		//printf("exit status: %d\n", (*parse_tree)->exit_status);
		if (parse_command(&command, parse_tree) == EXIT_FAILURE)
			exit ((*parse_tree)->exit_status);
		if (debug_mode)
			print_parse_tree(*parse_tree);
		execute_command(*parse_tree);
		free_tree(*parse_tree);
	}
	return ((*parse_tree)->exit_status);
}

int	main(int argc, char **argv, char **envp)
{
	t_tree	*parse_tree;
	int		shell_status;

	(void)argc;
	(void)argv;
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	parse_tree = (t_tree *)malloc(sizeof(t_tree));
	if (!parse_tree)
		return (1);
	parse_tree->exit_status = 0;
	parse_tree->signal_exit = 0;
	parse_tree->env = init_env_list(envp);
	parse_tree->parent_pipe = NULL;
	if (!parse_tree->env)
		return (free(parse_tree->env), 1);
	shell_status = prompt_loop(&parse_tree);
	free_env_list(parse_tree->env);
	free(parse_tree);
	clear_history();
	return (shell_status);
}
