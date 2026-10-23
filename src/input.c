#include "wordle.h"

static t_return_type	validate_word(char (*dict)[WORD_LEN + 1], char *input)
{
	uint32_t	i;
	uint8_t		j;
	size_t		len;

	len = strlen(input);	//TODO: simplify with just strncmp?
	if (len < WORD_LEN)
		return (RT_TOO_SHORT);
	if (len > WORD_LEN)
		return (RT_TOO_LONG);
	i = -1;
	while (++i < WORD_CNT)	//TODO: binary search or something faster?
	{
		j = -1;
		while (++j < WORD_LEN)
			input[j] = tolower(input[j]);
		if (!strncmp(dict[i], input, WORD_LEN))
			return (RT_FOUND);
	}
	return (RT_NOT_FOUND);
}

void	handle_user_input(t_ctx *c)
{
	t_return_type	ret;
	char			*read_line;

	while (true)
	{
		errno = 0;
		read_line = readline(PROMPT);
		if (!read_line)
		{
			if (errno)
				perror("handle_user_input");
			else
				write(1, "\n", 1);
			exit(EXIT_FAILURE);
		}
		ret = validate_word(c->dict, read_line);
		if (ret == RT_FOUND)
			break ;
		free(read_line);
		if (ret == RT_TOO_SHORT)
			write(1, WARN_TOO_SHORT, strlen(WARN_TOO_SHORT));
		else if (ret == RT_TOO_LONG)
			write(1, WARN_TOO_LONG, strlen(WARN_TOO_LONG));
		else
			write(1, WARN_UNKNOWN_WORD, strlen(WARN_UNKNOWN_WORD));
	}
	memcpy(c->guess[c->guess_cnt], read_line, WORD_LEN);
	free(read_line);
	check_guess(c);
}
