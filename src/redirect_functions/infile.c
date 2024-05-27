/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infile.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stopp <stopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 15:43:16 by stopp             #+#    #+#             */
/*   Updated: 2024/05/27 15:56:43 by stopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*update_cmdstr_in(char *cmdstr, int skip_len)
{
	int		i;
	int		j;
	char	*new_cmdstr;

	i = 0;
	j = 0;
	new_cmdstr = malloc(ft_strlen(cmdstr) - j + 1);
	if (!new_cmdstr)
		return (NULL);
	new_cmdstr[ft_strlen(cmdstr) - j] = '\0';
	while (cmdstr[i])
	{
		if (ft_strncmp(&cmdstr[i], "<", 1) == 0)
			j += skip_len;
		new_cmdstr[i++] = cmdstr[j++];
	}
	free(cmdstr);
	return (new_cmdstr);
}

char	*open_infile(t_tree *tree, char *cmdstr, char *infile)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	tree->in_fd = open(infile, O_RDONLY);
	if (!tree->in_fd)
		return (NULL);
	while (cmdstr[i])
	{
		if (ft_strncmp(&cmdstr[i], "<", 1) == 0)
		{
			j = 2;
			while (cmdstr[i + j] && cmdstr[i + j] == ' ')
				j++;
			j += ft_strlen(infile);
			break ;
		}
		i++;
	}
	cmdstr = update_cmdstr_in(cmdstr, j);
	return (cmdstr);
}

char	*handle_infile(char *cmd_str, t_tree *tree)
{
	char	*infile;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (cmd_str[i])
	{
		if (ft_strncmp(&cmd_str[i], "<", 1) == 0)
		{
			i += 1;
			while (cmd_str[i] && cmd_str[i] == ' ')
				i++;
			while (cmd_str[i + j] && cmd_str[i + j] != ' ')
				j++;
			infile = malloc(sizeof(char) * (j + 1));
			if (!infile)
				return (NULL);
			ft_strlcpy(infile, &cmd_str[i], j + 1);
			cmd_str = open_infile(tree, cmd_str, infile);
			free(infile);
		}
		i++;
	}
	return (cmd_str);
}
