/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stopp <stopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 11:03:04 by fkeitel           #+#    #+#             */
/*   Updated: 2024/05/15 12:27:35 by stopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*command;
	t_tree	*parse_tree;
	int		debug_mode;
	t_env	**env_lst;
	int		i;

	i = 0;
	command = NULL;
	(void)argc;
	(void)argv;
	debug_mode = 0;
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	env_lst = init_env_list(envp);
	if (!env_lst)
		return(1);
	// print_list(*env_lst);
	// printf("\n\n\n");
	// while (envp[i])
	// 	printf("%s\n", envp[i++]);
	// return (0);
	while (1)
	{
		command = readline("\033[32mminishell> \033[0m");
		if (command == NULL || strcmp(command, "exit") == 0)
		{
			if (command)
				free(command);
			break ;
		}
		if (command[0] && command[0] == '\n')
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
		add_history(command);
		parse_tree = parse_command(command, envp);
		free(command);
		if (parse_tree == NULL)
			continue ;
		if (debug_mode)
			print_parse_tree(parse_tree);
		execute_command(parse_tree, env_lst);
		free_tree(parse_tree);
	}
	clear_history();
	return (0);
}

//char	*args[MAX_ARGS];

	//command[strcspn(command, "\n")] = '\0';
	//g_history[g_history_count++] = strdup(command);
	//if (args[0] == NULL)
	//{
	//	continue ;
	//}
	//if (strcmp(args[0], "exit") == 0)
	//{
	//	break ;
	//}
	//if (strcmp(args[0], "history") == 0)
	//{
	//	while (i < g_history_count)
	//	{
	//		printf("%d: %s\n", i + 1, g_history[i]);
	//		i++;
	//	}
	//	continue ;
	//}
	//execute_command(args);

//while (i < g_history_count)
//{
//	free(g_history[i]);
//	i++;
//}
