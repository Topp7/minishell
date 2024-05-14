/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stopp <stopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 11:03:04 by fkeitel           #+#    #+#             */
/*   Updated: 2024/05/14 15:59:40 by stopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	*init_node(char *envp)
{
	int		i;
	int		j;
	t_env	*env;

	i = 0;
	env = malloc(sizeof(t_env));
	if (!env)
		return (NULL);
	while(envp[i] != '=' && envp[i])
		i++;
	env->name = malloc(i + 1);
	if (!env->name)
		return (NULL);
	ft_strlcpy(env->name, envp, i + 1);
	j = i + 1;
	while (envp[i])
		i++;
	env->value = malloc(i - j + 1);
	if (!env->name)
		return (NULL);
	ft_strlcpy(env->value, &envp[j], i - j + 1);
	env->next = NULL;
	return (env);
}

void	lstadd_back_env(t_env **lst, t_env *new)
{
	t_env	*current;

	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	current = *lst;
	while (current->next)
		current = current->next;
	current->next = new;
}

t_env	**init_env_list(char **envp)
{
	int		i;
	t_env	**env_lst;
	t_env	*env;

	i = 0;
	env = NULL;
	env_lst = malloc(sizeof(t_env *));
	if (!env_lst)
		return (NULL);
	i++;
	while (envp[i])
	{
		env = init_node(envp[i]);
		if (!env)
			return (NULL);
		lstadd_back_env(env_lst, env);
		i++;
	}
	return (env_lst);
}

// void	print_list(t_env *env_list)
// {
// 	while (env_list)
// 	{
// 		printf("%s=%s\n", env_list->name, env_list->value);
// 		env_list = env_list->next;
// 	}
// }

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
