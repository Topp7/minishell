/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkeitel <fkeitel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 10:38:16 by fkeitel           #+#    #+#             */
/*   Updated: 2024/05/20 17:27:15 by fkeitel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//	function to check for quotes in the env and fills the strings
int	check_for_quotes(char **replace, t_env *envp, char **var, char *arg)
{
	int		j;

	j = 0;
	while (envp->name[j] && *arg && arg[j] && envp->name[j] == arg[j])
		(j)++;
	if (!envp->name[j] && (!arg[j] || (arg[j] && (arg[j] == ' '
		|| arg[j] == '\'' || arg[j] == '\"' || arg[j] == '$'))))
	{
		*var = malloc(sizeof(char) * (ft_strlen(envp->name) + 2));
		if (!(*var))
			return (-1);
		(*var)[0] = '$';
		ft_strlcpy(*var + 1, envp->name, ft_strlen(envp->name) + 1);
		*replace = ft_strdup(envp->value);
		if (!(*replace))
			return (free(*var), -1);
		return (1);
	}
	return (0);
}

//	this function searches for a variable in the environment and saves in var
int	find_var_in_env(char **replace, t_env *envp, char *arg, char **var)
{
	t_env	*temp;
	int		quote_check;
	int		j;

	*var = NULL;
	j = 1;
	if (!arg || (arg && arg[0] != '$'))
		return (0);
	temp = envp;
	while (temp)
	{
		quote_check = check_for_quotes(replace, temp, var, arg + 1);
		if (quote_check != 0)
			return (quote_check);
		temp = temp->next;
	}
	while (arg[j] && arg[j] && arg[j] != ' ' && arg[j] != '$'
		&& arg[j] != '\"' && arg[j] != '\'')
		j++;
	if (j > 0)
	{
		*var = malloc(sizeof(char) * (j + 1));
		if (!var)
			return (-1);
		(*var)[0] = '$';
		ft_strlcpy(*var + 1, arg, j);
		*(replace) = malloc(sizeof(char));
		if (!(*replace))
			return (free(*var), -1);
		(*replace)[0] = '\0';
		return (1);
	}
	return (1);
}

//	function to remove single or double quotes form the arg string
void	remove_quotes(char **args, int i, int j)
{
	if (args[i][j - 1])
	{
		if (args[i] && args[i][j - 1] == '\'')
			remove_char(args[i], '\'', j - 1, &j);
		else if (args[i] && args[i][j - 1] == '\"')
			remove_char(args[i], '\"', j - 1, &j);
	}
	j = 1;
	if (args[i] && args[i][0] == '\'')
		remove_char(args[i], '\'', 0, &j);
	else if (args[i] && args[i][0] == '\"')
		remove_char(args[i], '\"', 0, &j);
}

int	exit_expand(char **arg, int *j, int exit_status)
{
	char	*exit_string;
	char	*dollar;

	if ((*arg)[(*j) + 1] && (*arg)[*j] == '$' && (*arg)[(*j) + 1] == '?')
	{
		exit_string = ft_itoa(exit_status);
		if (!exit_string)
			return (EXIT_FAILURE);
		dollar = ft_strdup("$?");
		if (!dollar)
			return (free(exit_string), EXIT_FAILURE);
		if (replace_substr(arg, &dollar, exit_string, j) == -1)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

//	function to convert the argument into the string
int	expander(char **args, t_env **env_lst, int exit_status)
{
	char	*var;
	char	*replace;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (args[i])
	{
		j = 0;
		while (args[i][j] != '\0')
		{
			if (quote_checker(args[i], j) && args[i][j] == '$'
				&& (exit_expand(&args[i], &j, exit_status) == EXIT_FAILURE
				|| find_var_in_env(&replace, *env_lst, args[i] + j, &var) == -1
				|| replace_substr(&args[i], &var, replace, &j) == -1))
				return (EXIT_FAILURE);
			j++;
		}
		remove_quotes(args, i, j);
		i++;
	}
	return (EXIT_SUCCESS);
}