/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkeitel <fkeitel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 10:47:36 by fkeitel           #+#    #+#             */
/*   Updated: 2024/05/30 17:07:27 by fkeitel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//	function to return an error string and free the tree and the array
int	pipes_error(char *errorstr, t_tree *tree, char **array)
{
	ft_printf("%s\n", errorstr);
	(void)array;
	free_two_dimensional_array(array);
	free_tree(tree);
	return (EXIT_FAILURE);
}

int	print_str_return_exit(char *str, int exit_code)
{
	dup2(2, 1);
	ft_printf(" %s", str);
	dup2(1, 1);
	return (exit_code);
}
