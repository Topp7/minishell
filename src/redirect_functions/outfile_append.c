/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   outfile_append.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkeitel <fkeitel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 16:43:57 by stopp             #+#    #+#             */
/*   Updated: 2024/05/28 18:04:00 by fkeitel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*update_cmdstr(char *cmdstr, int skip_len)
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
	while (cmdstr[j])
	{
		if (ft_strncmp(&cmdstr[j], ">>", 2) == 0)
		{
			j += skip_len;
			new_cmdstr[i++] = cmdstr[j];
		}
		else
			new_cmdstr[i++] = cmdstr[j++];
	}
	free(cmdstr);
	return (new_cmdstr);
}

char	*open_outfile(t_tree *tree, char *cmdstr, char *outfile)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	tree->out_fd = open(outfile, O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (!tree->out_fd)
		return (NULL);
	while (cmdstr[i])
	{
		if (ft_strncmp(&cmdstr[i], ">>", 2) == 0)
		{
			j = 2;
			while (cmdstr[i + j] && cmdstr[i + j] == ' ')
				j++;
			j += ft_strlen(outfile);
			break ;
		}
		i++;
	}
	cmdstr = update_cmdstr(cmdstr, j);
	free(outfile);
	return (cmdstr);
}

char	*handle_append(char *cmdstr, t_tree *tree)
{
	int		i;
	int		j;
	char	*outfile;

	i = 0;
	j = 0;
	while (cmdstr[i])
	{
		if (ft_strncmp(&cmdstr[i], ">>", 2) == 0)
		{
			i += 2;
			while (cmdstr[i] && cmdstr[i] == ' ')
				i++;
			while (cmdstr[i + j] && cmdstr[i + j] != ' ')
				j++;
			outfile = malloc(sizeof(char) * (j + 1));
			if (!outfile)
				return (NULL);
			ft_strlcpy(outfile, &cmdstr[i], j + 1);
			cmdstr = open_outfile(tree, cmdstr, outfile);
			break ;
		}
		i++;
	}
	return (cmdstr);
}
