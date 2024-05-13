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

size_t	ft_strlen(const char *c)
{
	size_t	counter;

	counter = 0;
	while (*c != '\0')
	{
		counter++;
		c++;
	}
	return (counter);
}

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	int	i;

	if (dst == NULL && src == NULL)
		return (NULL);
	if (dst > src)
	{
		i = (int)len - 1;
		while (i >= 0)
		{
			*(char *)(dst + i) = *(char *)(src + i);
			i--;
		}
	}
	else
	{
		i = 0;
		while (i < (int)len)
		{
			*((char *)(dst + i)) = *((char *)(src + i));
			i++;
		}
	}
	return (dst);
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned int	counter;
	char			*char_dst;
	char			*char_src;

	counter = 0;
	char_dst = (char *)dst;
	char_src = (char *)src;
	if (char_dst == NULL && char_src == NULL)
		return (NULL);
	while (char_src[counter] && counter < n)
	{
		char_dst[counter] = char_src[counter];
		counter++;
	}
	return (dst);
}

//	this function allocates the new substring for the string replace in a string
int	alloc_string(char **s, int result_len)
{
	char	*temp;
	int		str_len;

	temp = NULL;
	str_len = (int)ft_strlen(*s);
	if (result_len != str_len)
	{
		temp = (char *)malloc(sizeof(char) * (result_len + 1));
		if (!temp)
			return (EXIT_FAILURE);
		ft_memcpy(temp, *s, str_len);
		temp[str_len] = '\0';
		free(*s);
		*s = temp;
	}
	return (EXIT_SUCCESS);
}

//	this function replaces in the str s the str_replace with the new_str
int	replace_var(char **s, char *str_replace, char *new_str, int *start)
{
	char	*suffix_pos;
	char	*substr_pos;
	int		rep_len;
	int		len_new_st;

	rep_len = ft_strlen(str_replace);
	len_new_st = ft_strlen(new_str);
	if (alloc_string(s, ft_strlen(*s) - rep_len + len_new_st) == EXIT_FAILURE)
	{
		free(new_str);
		return (0);
	}
	substr_pos = *s + *start;
	if (substr_pos == NULL)
		return (0);
	suffix_pos = substr_pos + rep_len;
	ft_memmove(substr_pos + len_new_st, substr_pos + rep_len, ft_strlen(suffix_pos) + 1);
	ft_memcpy(substr_pos, new_str, len_new_st);
	free(str_replace);
	free(new_str);
	*start += len_new_st;
	return (1);
}

// Test function
void	test_replace_substr(void)
{
	int		success;
	char	*original_string;
	char	*substring;
	char	*replacement;
	int		i;

	i = 4;
	original_string = strdup("  $PWD . ");
	substring = strdup("$P");
	replacement = strdup("test/file");
	if (original_string == NULL || substring == NULL || replacement == NULL)
	{
		printf("Memory allocation failed!\n");
		return ;
	}
	printf("Original string: [%s]\n", original_string);
	success = replace_var(&original_string, substring, replacement, &i);
	if (success)
		printf("After replacement: [%s]\n", original_string);
	else
		printf("Replacement failed!\n"), exit(1);
	free(original_string);
}

int	main(void)
{
	test_replace_substr();
	return (0);
}
