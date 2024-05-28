/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stopp <stopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 10:47:36 by fkeitel           #+#    #+#             */
/*   Updated: 2024/05/28 15:41:38 by stopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void signal_handle(int signo)
{
	(void)signo;
	// rl_catch_signals = 0;
	if (signo == SIGQUIT)
	{
		ft_printf("QUIT: 3\n");
		exit(131);
	}
	if (signo == SIGINT)
		exit(130);
}

//	^C clears current input line, redraws prompt, and moves cursor to a new line
void	signal_handler(int sig)
{
	(void)sig;
	//rl_catch_signals = 0;
	write(1, "\n", 1);
	rl_replace_line("", 1);
	rl_on_new_line();
	rl_redisplay();
	return ;
}
