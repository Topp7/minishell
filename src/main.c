/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkeitel <fkeitel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 11:03:04 by fkeitel           #+#    #+#             */
/*   Updated: 2024/05/15 17:41:36 by fkeitel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*command;
	t_tree	*parse_tree;
	int		debug_mode;
	t_env	**env_lst;

	command = NULL;
	(void)argc;
	(void)argv;
	debug_mode = 0;
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	env_lst = init_env_list(envp);
	if (!env_lst)
		return (1);
	while (1)
	{
		command = readline("\033[32mminishell> \033[0m");
		if (command == NULL)
			break ;
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
		parse_tree = parse_command(&command, env_lst);
		if (parse_tree == NULL)
			return (1);
		if (debug_mode)
			print_parse_tree(parse_tree);
		execute_command(parse_tree);
		free_tree(parse_tree, 0);
	}
	free_env_list(env_lst);
	clear_history();
	return (0);
}
