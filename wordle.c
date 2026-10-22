/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wordle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 21:53:51 by sancuta           #+#    #+#             */
/*   Updated: 2026/10/22 05:40:26 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "worlde.h"

bool	get_word(int fd, char *input)
{
	uint32_t	read_len;

	read_len = read(fd, input, WORD_LEN);
	if (!read_len)
		return (false);
	while (read_len == -1)
	{
		if (errno == EINTR)
			read_len = read(fd, input, WORD_LEN);
		handle_exit(/*some parameters*/);
	}
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
			fd = open(PATH, O_RDONLY);
		handle_exit(/*some parameters*/);
	}

	i = -1;
	while (get_word(fd, c->dict[++i]))
		;
}

void	select_target(t_ctx *c)
{
	struct tm	*time;
	uint32_t	target_idx;

	time = localtime(&time(NULL));
	target_idx = (time.tm_year * SEED + time.tm_yday) % WORD_CNT;
}

t_return_type	validate_word(char **dict, char *input)
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
	uint8_t	i;
	uint8_t	j;
	uint8_t	guess_mask;
	uint8_t	current_bit;

	guess_mask = 255	// 0b11111111

	i = -1;
	while(++i < WORD_LEN)
	{
		if (c->target[i] == (*c->guess)[i])
		{
			current_bit = 1 << i;
			(*c->type)[i] = MT_MATCH;
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
			if (c->target[j] == (*c->guess)[i] && guess_mask & current_bit)
			{
				(*c->type)[i] = MT_WRONG_POS;
				guess_mask ^= current_bit;
				break ;
			}
		}
	}
}

void	handle_user_input(t_ctx *c, uint8_t guess_nbr)
{
	t_return_type	ret;
	t_match_type	match;
	char			*prompt;

	c->guess[guess_nbr] = readline(PROMPT);
	ret = validate_word(c->dict, c->guess[guess_nbr]);
	while (ret != RT_FOUND)
	{
		if (ret == RT_TOO_SHORT)
			write(1, WARN_TOO_SHORT, strlen(WARN_TOO_SHORT));
		else if (ret == RT_TOO_LONG)
			write(1, WARN_TOO_LONG, strlen(WARN_TOO_LONG));
		else
			write(1, WARN_UNKNOWN_WORD, strlen(WARN_UNKOWN_WORD));
		c->guess[guess_nbr] = readline(PROMPT);
		ret = validate_word(c->dict, c->guess[guess_nbr]);
	}
	check_guess(c);
}

bool	check_victory(t_ctx *c)
{
	uint8_t	i;
	char	cnt;

	cnt = c->guess_cnt + '0';
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
	return (true)
}

void	init_ctx(t_ctx *c)
{
	c->target_idx = 0;
	memset(c->guess, '_', MAX_GUESS * WORD_LEN);
	c->guess_cnt = -1;
	read_dict(c);
	select_target(c);
}

void	print_field_guesses(t_ctx *c)
{
	uint8_t	i;
	uint8_t	j;
	char	field_line[FIELD_LEN];

	i = -1;
	while (++i < c->guess_cnt)
	{
		j = -1;
		while (++j < FIELD_LEN) //TODO: this doesnt work, probably
		{
			if (c->type[i][j] == MT_NO_MATCH)
				field_line[j] = RESET;
			else if (c->type[i][j] == MT_MATCH)
				field_line[j] = GREEN;
			else if (c->type[i][j] == MT_WRONG_POS)
				field_line[j] = YELLOW;
			field_line[j + 1] = c->guess[i][j++];
			if (++j != FIELD_LEN - 1)
				field_line[j] = ' ';
			else
				field_line[j] = '\n';
		}
	}
}

int	main()
{
	t_ctx	c;

	init_ctx(&c);
	while (++c.guess_cnt <= MAX_GUESS)
	{
		handle_user_input(&c);
		if (check_victory(&c))
			return (0);
		print_field_guesses(&c);
		print_field_blanks(&c);
	}
	return (0);
}
