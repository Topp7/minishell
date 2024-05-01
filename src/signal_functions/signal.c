/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkeitel <fkeitel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 10:47:36 by fkeitel           #+#    #+#             */
/*   Updated: 2024/04/30 09:17:38 by fkeitel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//	^C clears current input line, redraws prompt, and moves cursor to a new line
void	signal_handler(int sig)
{
	(void)sig;
	rl_replace_line("", 0);
	rl_redisplay();
	ft_putstr_fd("\n\033[32mMinishell> \033[0m", 1);
}