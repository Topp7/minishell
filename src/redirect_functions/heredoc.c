/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stopp <stopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 16:06:36 by stopp             #+#    #+#             */
/*   Updated: 2024/05/28 15:43:06 by stopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	skip_here(int *i, char *str, char *here_doc)
{
	if (ft_strncmp(&str[*i], "<<", 2) == 0)
	{
		*i += 2;
		while (str[*i] == ' ')
			*i += 1;
		*i += ft_strlen(here_doc);
		return (1);
	}
	else
		return (0);
}

char	*create_str(char *str, char *here_doc)
{
	int		i;
	int		j;
	char	*new_str;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (skip_here(&i, str, here_doc) == 0)
		{
			j++;
			i++;
		}
	}
	new_str = malloc(j + 1);
	new_str[j] = '\0';
	i = 0;
	j = 0;
	while (str[i])
	{
		if (skip_here(&i, str, here_doc) == 0)
			new_str[j++] = str[i++];
	}
	free (str);
	return (new_str);
}

char	*create_heredoc(char *str, char *cmd_str, t_tree *tree, int *ex_st)
{
	int		fd[2];
	int		pid;
	char	*buf;
	char	*new_cmdstr;

	buf = NULL;
	if (pipe(fd) == -1)
		return (NULL);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, signal_handle);
		while (1)
		{
			signal(SIGQUIT, SIG_IGN);
			buf = get_next_line(STDIN_FILENO);
			if (buf == NULL)
				break ;
			if (ft_strncmp(buf, str, ft_strlen(buf)) == 10)
				break ;
			free(buf);
		}
		free(buf);
		exit (tree->exit_status);
	}
	waitpid(pid, &tree->exit_status, 0);
	if (WIFEXITED(tree->exit_status))
		tree->exit_status = WEXITSTATUS(tree->exit_status);
	printf("tree: %d ex: %d\n", tree->exit_status, *ex_st);
	close(fd[1]);
	tree->in_fd = fd[0];
	new_cmdstr = create_str(cmd_str, str);
	return (new_cmdstr);
}

//	function to save the heredoc input in a string
char	*handle_heredoc(char *cmd_str, t_tree *tree, int *ex_st)
{
	char	*here_str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (cmd_str[i])
	{
		if (ft_strncmp(&cmd_str[i], "<<", 2) == 0)
		{
			i += 2;
			while (cmd_str[i] && cmd_str[i] == ' ')
				i++;
			while (cmd_str[i + j] && cmd_str[i + j] != ' ')
				j++;
			here_str = malloc(sizeof(char) * (j + 1));
			if (!here_str)
				return (NULL);
			ft_strlcpy(here_str, &cmd_str[i], j + 1);
			cmd_str = create_heredoc(here_str, cmd_str, tree, ex_st);
			free(here_str);
			i = 0;
		}
		i++;
	}
	return (cmd_str);
}
