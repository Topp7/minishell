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
int	alloc_string(char **s, char **substr_pos, char *str_replace, int result_len)
{
	char	*temp;

	if (result_len != (int)ft_strlen(*s))
	{
		temp = malloc(result_len + 1);
		if (!temp)
			return (EXIT_FAILURE);
		ft_memcpy(temp, *s, result_len - 1);
		temp[result_len] = '\0';
		free(*s);
		*s = temp;
		*substr_pos = strstr(*s, str_replace);
		if (*substr_pos == NULL)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

//	this function replaces in the str s the str_replace with the new_str
int	replace__var(char **s, char *str_replace, char *new_str, int *start)
{
	int		replace_len;
	char	*substr_pos;
	int		suf_len;
	int		len_new_st;
	int		result_len;

	replace_len = ft_strlen(str_replace);
	substr_pos = *s + *start;
	if (substr_pos == NULL)
		return (0);
	suf_len = ft_strlen(substr_pos + replace_len);
	len_new_st = ft_strlen(new_str);
	result_len = ft_strlen(*s) - replace_len + len_new_st;
	printf("hello\n");
	printf("\ns: [%s] substr_pos: [%s] s_len: %d result_len: %d \n",
		*s, substr_pos, (int)ft_strlen(*s), result_len);
	if (alloc_string(s, &substr_pos, str_replace, result_len) == EXIT_FAILURE)
	{
		free(str_replace);
		free(new_str);
		return (0);
	}
	ft_memmove(substr_pos + len_new_st, substr_pos + replace_len, suf_len);
	ft_memcpy(substr_pos, new_str, len_new_st);
	free(str_replace);
	free(new_str);
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

	i = 0;
	original_string = strdup("Hello  '$PWD'  Hello  ");
	substring = strdup("$PWD");
	replacement = strdup("test/file");
	if (original_string == NULL || substring == NULL || replacement == NULL)
	{
		printf("Memory allocation failed!\n");
		return ;
	}
	printf("Original string: [%s]\n", original_string);
	success = replace__var(&original_string, substring, replacement, &i);
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
