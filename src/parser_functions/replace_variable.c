/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_variable.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkeitel <fkeitel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 10:38:16 by fkeitel           #+#    #+#             */
/*   Updated: 2024/05/15 12:31:18 by fkeitel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//	this function searches for a variable in the environment and saves in var
int	search_for_var_in_env(char **s, char **envp, char *arg, char **var)
{
	size_t	len;
	char	*new_str;
	int		i;
	int		j;

	*s = strdup(arg);
	*var = strdup(*s);
	i = -1;
	while (envp[++i])
	{
		j = 0;
		while (envp[i][j] && (*s)[j + 1] && envp[i][j] == (*s)[j + 1])
			j++;
		if (envp[i][j] && envp[i][j] == '=' && (!(*s)[j + 1] || ((*s)[j + 1]
			&& ((*s)[j + 1] == ' ' || (*s)[j + 1] == '\'' || (*s)[j + 1] == '\"'))))
		{
			free(*var);
			*var = malloc(sizeof(char) * (j + 2));
			ft_strlcpy(*var, *s, j + 2);
			len = strlen(envp[i] + ++j);
			new_str = ft_substr(envp[i], j, len);
			if (new_str != NULL)
			{
				free(*s);
				*s = ft_strdup(new_str);
				return (free(new_str), 1);
			}
			free(*var);
			return (0);
		}
	}
	if ((*s)[j + 1])
	{
		free(*s);
		*s = malloc(sizeof(char));
		*s[0] = '\0';
		return (1);
	}
	free(*var);
	return (0);
}

//	this function allocates the new substring for the string replace in a string
int	alloc_string(char **s, int result_len)
{
	char	*temp;
	int		str_len;

	temp = NULL;
	str_len = (int)ft_strlen(*s);
	if (result_len != str_len)
	{
		temp = (char *)malloc(sizeof(char) * (result_len + 1));
		if (!temp)
			return (EXIT_FAILURE);
		ft_memcpy(temp, *s, str_len);
		temp[str_len] = '\0';
		free(*s);
		*s = temp;
	}
	return (EXIT_SUCCESS);
}

//	this function replaces in the str s the str_replace with the new_str
int	replace_var(char **s, char **str_replace, char *new_str, int *start)
{
	char	*suffix_pos;
	char	*substr_pos;
	int		rep_len;
	int		len_new_st;

	rep_len = ft_strlen(*str_replace);
	free(*str_replace);
	len_new_st = ft_strlen(new_str);
	if (alloc_string(s, ft_strlen(*s) - rep_len + len_new_st) == EXIT_FAILURE)
	{
		free(new_str);
		return (0);
	}
	substr_pos = *s + *start;
	if (substr_pos == NULL)
		return (0);
	suffix_pos = substr_pos + rep_len;
	ft_memmove(substr_pos + len_new_st, substr_pos + rep_len,
		ft_strlen(suffix_pos) + 1);
	ft_memcpy(substr_pos, new_str, len_new_st);
	free(new_str);
	*start += - rep_len + len_new_st;
	return (1);
}

int	quote_checker(char *arg, int j)
{
	static int	single_quote = 0;
	static int	double_quote = 0;

	if (j == 0)
	{
		single_quote = 0;
		double_quote = 0;
	}
	if (arg[j] && arg[j] == '\'' && !(double_quote))
	{
		single_quote = !(single_quote);
	}
	else if (arg[j] && arg[j] == '\"' && !(single_quote))
	{
		double_quote = !(double_quote);
	}
	if (single_quote)
		return (0);
	return (1);
}

//	function to convert the argument into the string
int	export_dollar_sign(char **args, char **env)
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
				&& search_for_var_in_env(&replace, env, args[i] + j, &var)
				&& !replace_var(&args[i], &var, replace, &j))
				return (EXIT_FAILURE);
			j++;
		}
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
		i++;
	}
	free_two_dimensional_array(env);
	return (EXIT_SUCCESS);
}
