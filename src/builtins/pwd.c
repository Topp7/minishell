/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stopp <stopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 15:32:06 by stopp             #+#    #+#             */
/*   Updated: 2024/05/15 17:23:33 by stopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_pwd(void)
{
	char	*buf;

	buf = getcwd(NULL, 0);
	write(STDOUT_FILENO, buf, ft_strlen(buf));
	write(1, "\n", 1);
	free(buf);
	// printf("test\n");
	return ;
}
