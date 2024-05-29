/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkeitel <fkeitel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 16:33:56 by fkeitel           #+#    #+#             */
/*   Updated: 2024/05/29 14:36:01 by fkeitel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//	public libraries
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <termios.h>
#include <readline/readline.h>
#include <readline/history.h>

//size_t	ft_strlen(const char *c)
//{
//	size_t	counter;

//	counter = 0;
//	if (!(*c))
//		return (0);
//	while (c[counter])
//	{
//		counter++;
//	}
//	return (counter);
//}

//void	*ft_memcpy(void *dst, const void *src, size_t n)
//{
//	unsigned int	counter;
//	char			*char_dst;
//	char			*char_src;

//	counter = 0;
//	char_dst = (char *)dst;
//	char_src = (char *)src;
//	if (char_dst == NULL && char_src == NULL)
//		return (NULL);
//	while (char_src[counter] && counter < n)
//	{
//		char_dst[counter] = char_src[counter];
//		counter++;
//	}
//	return (dst);
//}

//void	*ft_memmove(void *dst, const void *src, size_t len)
//{
//	int	i;

//	if (dst == NULL && src == NULL)
//		return (NULL);
//	if (dst > src)
//	{
//		i = (int)len - 1;
//		while (i >= 0)
//		{
//			*(char *)(dst + i) = *(char *)(src + i);
//			i--;
//		}
//	}
//	else
//	{
//		i = 0;
//		while (i < (int)len)
//		{
//			*((char *)(dst + i)) = *((char *)(src + i));
//			i++;
//		}
//	}
//	return (dst);
//}

////	this function allocates the new substring for the string
//int	alloc_string(char **s, int result_len)
//{
//	char	*temp;
//	int		str_len;

//	temp = NULL;
//	str_len = (int)ft_strlen(*s);
//	if (result_len >= str_len)
//	{
//		temp = (char *)malloc(sizeof(char) * (result_len + 1));
//		if (!temp)
//			return (EXIT_FAILURE);
//		ft_memcpy(temp, *s, str_len);
//		temp[str_len] = '\0';
//		free(*s);
//		*s = temp;
//	}
//	return (EXIT_SUCCESS);
//}

////	this function replaces in the str s the sub_str with the new_str
//int	replace_substr(char **s, char **sub_str, char *new_str, int *position)
//{
//	char	*substr_pos;
//	int		rep_len;
//	int		len_new_st;

//	if (!(*sub_str) || !(*s))
//		return (0);
//	rep_len = ft_strlen(*sub_str);
//	free(*sub_str);
//	len_new_st = ft_strlen(new_str);
//	if (alloc_string(s, ft_strlen(*s) - rep_len + len_new_st) == EXIT_FAILURE)
//	{
//		free(new_str);
//		return (-1);
//	}
//	substr_pos = *s + *position;
//	if (!substr_pos || substr_pos < *s)
//		return (0);
//	ft_memmove(substr_pos + len_new_st, substr_pos + rep_len,
//		ft_strlen(substr_pos + rep_len) + 1);
//	ft_memcpy(substr_pos, new_str, len_new_st);
//	free(new_str);
//	if (alloc_string(s, ft_strlen(*s)) == EXIT_FAILURE)
//		return (-1);
//	return (1);
//}

//#include <string.h>

//int	main(void)
//{
//	char *s = strdup("echo $PWD sr");
//	char *sub_str = strdup("$PWD");
//	char *new_str = strdup("0dgjkg");
//	int i = 5;
//	if (replace_substr(&s, &sub_str, new_str, &i) == -1)
//		return (-1);
//	printf("%s\n", s);
//	free(s);
//}

#include <stdio.h>

int main(int argc, char *argv[]) {
    // argc contains the number of arguments passed, including the program name
    printf("Number of arguments: %d\n", argc - 1);

    return 0;
}