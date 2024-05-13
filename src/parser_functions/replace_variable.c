/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_variable.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkeitel <fkeitel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 10:38:16 by fkeitel           #+#    #+#             */
/*   Updated: 2024/05/13 16:22:18 by fkeitel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//	this function searches for a variable in the environment and saves in var
int	search_for_variable_in_env(char **s, char **envp, char *arg, char **var)
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
		if (envp[i][j] && envp[i][j] == '=')
		{
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
			return (0);
		}
	}
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
int	replace_var(char **s, char *str_replace, char *new_str, int *start)
{
	char	*suffix_pos;
	char	*substr_pos;
	int		rep_len;
	int		len_new_st;

	rep_len = ft_strlen(str_replace);
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
	ft_memmove(substr_pos + len_new_st, substr_pos + rep_len, ft_strlen(suffix_pos) + 1);
	ft_memcpy(substr_pos, new_str, len_new_st);
	free(new_str);
	*start += len_new_st;
	return (1);
}

//	function to convert the argument into the string
int	export_dollar_sign(char **args, char **envp)
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
		while (args[i][j])
		{
			if (args[i][j] == '$')
			{
				if (search_for_variable_in_env(&replace, envp, args[i] + j, &var))
				{
					if (!replace_var(&args[i], var, replace, &j))
						return (EXIT_FAILURE);
				}
			}
			j++;
		}
		i++;
	}
	return (EXIT_SUCCESS);
}
