/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirec_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkeitel <fkeitel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 21:56:38 by fkeitel           #+#    #+#             */
/*   Updated: 2024/06/02 20:02:35 by fkeitel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	check_cat(char *str)
{
	int	i;

	i = 3;
	while (str[i])
	{
		if (str[i] != ' ' || str[i] != '\t')
			return (1);
		i++;
	}
	return (0);
}

char	**cpy_args(char **new, char ***args)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while ((*args)[i])
	{
		if ((*args)[i][0] != '\0')
		{
			new[j] = ft_strdup((*args)[i]);
			if (!new[j])
			{
				free_two_dimensional_array(new);
				return (NULL);
			}
			j++;
		}
		i++;
	}
	new[j] = NULL;
	free_two_dimensional_array(*args);
	return (new);
}

int	update_args(char ***args)
{
	char	**new;
	int		count;
	int		i;

	count = 0;
	i = 0;
	while ((*args)[i])
	{
		if ((*args)[i][0] != '\0')
			count++;
		i++;
	}
	new = malloc(sizeof(char *) * (count + 1));
	if (!new)
		return (-1);
	new[count] = NULL;
	new = cpy_args(new, args);
	*args = new;
	return (-1);
}
