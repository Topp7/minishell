/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_variable.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkeitel <fkeitel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 10:38:16 by fkeitel           #+#    #+#             */
/*   Updated: 2024/05/01 12:17:56 by fkeitel          ###   ########.fr       */
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
			len = strlen(envp[i] + ++j);
			new_str = ft_substr(envp[i], j, len);
			if (new_str != NULL)
			{
				free(*s);
				*s = ft_strdup(new_str);
				return (free(new_str), 1);
			}
			else
				return (0);
		}
	}
	return (0);
}

//	this function allocates the new substring for the string replace in a string
int	alloc_string(char **s, char **substr_pos, char *str_replace, int result_len)
{
	char	*temp;

	if (result_len > (int)ft_strlen(*s))
	{
		temp = realloc(*s, result_len + 1);
		if (!temp)
			return (EXIT_FAILURE);
		*s = temp;
		*substr_pos = strstr(*s, str_replace);
		if (*substr_pos == NULL)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

//	this function replaces in the str s the str_replace with the new_str
int	replace__var(char **s, char *str_replace, char *new_str, int *start)
{
	int		replace_len;
	char	*substr_pos;
	int		suf_len;
	int		result_len;
	int		len_new_st;

	(void)start;
	printf("%s slen: %ld\n", *s, ft_strlen(*s));
	len_new_st = ft_strlen(new_str);
	replace_len = ft_strlen(str_replace);
	substr_pos = strstr(*s + *start, str_replace);
	if (substr_pos == NULL)
		return (0);
	suf_len = ft_strlen(substr_pos + replace_len);
    result_len = *start + len_new_st + suf_len;
	if (alloc_string(s, &substr_pos, str_replace, result_len) == EXIT_FAILURE)
	{
		free(str_replace);
		free(new_str);
		return (0);
	}
	ft_memmove(substr_pos + len_new_st, substr_pos + replace_len, suf_len + 1);
	ft_memcpy(substr_pos, new_str, len_new_st);
	printf("slen: %ld\n", ft_strlen(substr_pos));
	free(str_replace);
	free(new_str);
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
					if (!replace__var(&args[i], var, replace, &j))
						return (EXIT_FAILURE);
					else
						printf("%s\n", args[i]);
				}
			}
			j++;
		}
		i++;
	}
	return (EXIT_SUCCESS);
}
