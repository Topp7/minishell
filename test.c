//	public libraries
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <errno.h>
# include <termios.h>
# include <readline/readline.h>
# include <readline/history.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function prototype
int replace_substr(char **s, char *str_to_replace, char *new_string, int start)
{
    int s_len = strlen(*s);
    int replace_len = strlen(str_to_replace);
    int new_len = strlen(new_string);
    char *substr_pos;

    if (start < 0 || start >= s_len)
        return 0;
    substr_pos = strstr(*s + start, str_to_replace);
    if (substr_pos == NULL)
        return 0;
    int suffix_len = s_len - (substr_pos - *s) - replace_len;

    int result_len = (substr_pos - *s) + new_len + suffix_len;

    if (result_len > s_len)
    {
        char *temp = realloc(*s, result_len + 1);
        if (!temp)
            return 0;
        *s = temp;
        substr_pos = strstr(*s + start, str_to_replace);
        if (substr_pos == NULL)
            return 0;
    }
    memmove(substr_pos + new_len, substr_pos + replace_len, suffix_len + 1);
    memcpy(substr_pos, new_string, new_len);
    return 1;
}

// Test function
void test_replace_substr()
{
    char *original_string = strdup("  $PWD  '''  ");
    char *substring = strdup("$PWD");
    char *replacement = strdup("/dorker_workspace/miniiii/minishell");

    if (original_string == NULL || substring == NULL || replacement == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    printf("Original string: %s\n", original_string);

    int success = replace_substr(&original_string, substring, replacement, 0);

    if (success) {
        printf("After replacement: [%s]\n", original_string);
    } else {
        printf("Replacement failed!\n");
    }

    // Free allocated memory
    free(original_string);
    free(substring);
    free(replacement);
}

int main()
{
	test_replace_substr();
	return 0;
}