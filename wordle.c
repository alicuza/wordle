/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wordle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 21:53:51 by sancuta           #+#    #+#             */
/*   Updated: 2026/10/22 09:48:36 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wordle.h"

bool	get_word(int fd, char *input)
{
	ssize_t	read_len;

	errno = 0;
	read_len = read(fd, input, WORD_LEN + 1);
	while (read_len == -1)
	{
		if (errno != EINTR)
		{
			perror("get_word");
			exit(EXIT_FAILURE);
		}
		errno = 0;
		read_len = read(fd, input, WORD_LEN + 1);
	}
	if (read_len <= 0)
		return (false);
	input[WORD_LEN] = '\0';
	return (true);
}

void	read_dict(t_ctx *c)
{
	int			fd;
	uint32_t	i;

	errno = 0;
	fd = open(PATH, O_RDONLY);
	while (fd == -1)
	{
		if (errno == EINTR)
		{
			fd = open(PATH, O_RDONLY);
			errno = 0;
		}
		else
		{
			perror("read_dict");
			exit(EXIT_FAILURE);
		}
	}

	i = -1;
	while (++i < WORD_CNT && get_word(fd, c->dict[i]))
		;
	close(fd);
}

void	select_target(t_ctx *c)
{
	struct tm	tm;
	time_t		t;

	t = time(NULL);
	tm = *localtime(&t);
	c->target_idx = (tm.tm_year * SEED + tm.tm_yday) % WORD_CNT;
}

t_return_type	validate_word(char (*dict)[WORD_LEN + 1], char *input)
{
	uint32_t	i;
	size_t		len;

	len = strlen(input);	//TODO: simplify with just strncmp?
	if (len < WORD_LEN)
		return (RT_TOO_SHORT);
	if (len > WORD_LEN)
		return (RT_TOO_LONG);
	i = -1;
	while (++i < WORD_CNT)	//TODO: binary search or something faster?
	{
		if (!strncmp(dict[i], input, WORD_LEN))
			return (RT_FOUND);
	}
	return (RT_NOT_FOUND);
}

void	check_guess(t_ctx *c)
{
	uint8_t			i;
	uint8_t			j;
	uint8_t			guess_mask;
	uint8_t			current_bit;
	char			*guess;
	t_match_type	*mt;

	guess_mask = 255;	// 0b11111111
	guess = c->guess[c->guess_cnt];
	mt = c->type[c->guess_cnt];
	i = -1;
	while(++i < WORD_LEN)
	{
		if (c->dict[c->target_idx][i] == guess[i])
		{
			current_bit = 1 << i;
			mt[i] = MT_MATCH;
			guess_mask ^= current_bit;
		}
	}

	i = -1;
	while(++i < WORD_LEN)
	{
		j = -1;
		while (++j < WORD_LEN)
		{
			current_bit = 1 << j;
			if (c->dict[c->target_idx][j] == guess[i]
				&& guess_mask & current_bit)
			{
				mt[i] = MT_WRONG_POS;
				guess_mask ^= current_bit;
				break ;
			}
		}
	}
}

void	handle_user_input(t_ctx *c)
{
	t_return_type	ret;
	char			*read_line;

	while (true)
	{
		read_line = readline(PROMPT);
		if (!read_line)
		{
			perror("handle_user_input");
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

bool	check_victory(t_ctx *c)
{
	uint8_t	i;
	char	cnt;

	cnt = c->guess_cnt + '1';
	i = -1;
	while (++i < WORD_LEN)
	{
		if (c->type[c->guess_cnt][i] != MT_MATCH)
			return (false);
	}
	write(1, VICTORY, strlen(VICTORY));
	write(1, c->guess[c->guess_cnt], WORD_LEN);
	write(1, VICTORY2, strlen(VICTORY2));
	write(1, &cnt, 1);
	write(1, VICTORY3, strlen(VICTORY3));
	return (true);
}

void	init_ctx(t_ctx *c)
{
	uint8_t	i;

	memset(c, 0, sizeof(t_ctx));
	i = -1;
	while (++i < MAX_GUESS)
	{
		memset(c->guess[i], '_', WORD_LEN);
		c->guess[i][WORD_LEN] = '\0';
	}
	c->guess_cnt = -1;
	read_dict(c);
	select_target(c);
}

void	print_field_guesses(t_ctx *c)
{
	uint8_t	i;
	uint8_t	j;

	i = -1;
	while (++i <= c->guess_cnt)
	{
		j = -1;
		while (++j < WORD_LEN)
		{
			write(1, " ", 1);
			if (c->type[i][j] == MT_MATCH)
				write(1, GREEN, strlen(GREEN));
			else if (c->type[i][j] == MT_WRONG_POS)
				write(1, YELLOW, strlen(YELLOW));
			write(1, &c->guess[i][j], 1);
			write(1, RESET, strlen(RESET));
			if (j == WORD_LEN - 1)
				write(1, "\n", 1);
		}
	}
}

void	print_field_blanks(t_ctx *c)
{
	uint8_t	i;

	i = c->guess_cnt;
	while (++i < MAX_GUESS)
		write(1, BLANK_FIELD, strlen(BLANK_FIELD));
	write(1, "\n", 1);
}

void	print_loss(t_ctx *c)
{
	write(1, LOSS, strlen(LOSS));
	write(1, c->dict[c->target_idx], WORD_LEN);
	write(1, LOSS2, strlen(LOSS2));
}

int	main()
{
	t_ctx	c;

	init_ctx(&c);
	print_field_blanks(&c);
	while (++c.guess_cnt < MAX_GUESS)
	{
		handle_user_input(&c);
		print_field_guesses(&c);
		print_field_blanks(&c);
		if (check_victory(&c))
			return (0);
	}
	print_loss(&c);
	return (0);
}
