/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wordle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 21:53:51 by sancuta           #+#    #+#             */
/*   Updated: 2026/10/22 00:46:49 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "worlde.h"

bool	get_word(int fd, char *input)
{
	uint32	read_len;

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
	int		fd;
	uint32	i;

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
	uint32		target_idx;

	time = localtime(&time(NULL));
	target_idx = (time.tm_year * SEED + time.tm_yday) % WORD_CNT;
}

t_match_type	check_guess(target, pos, guess.ch)
{
	uint8	i;

	if (target[pos] == guess.ch)
		return (MT_MATCH);
	i = -1;
	while (i < WORD_LEN)
	{
		if (target[++i] == guess.ch)
			return (MT_WRONG_POS);
	}
	return (MT_NO_MATCH);
}

t_return_type	validate_word(char **dict, char *input)
{
	uint32	i;
	size_t	len;

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

void	handle_user_input(t_ctx *c)
{
	t_return_type	ret;

	c->guess = readline(PROMPT);
	ret = validate_word(c->dict, c->guess);
	while (ret == RT_TOO_SHORT || ret == RT_TOO_LONG)
	{
		c->guess = readline(PROMPT);
		ret = validate_word(c->dict, c->guess);
	}
	if (ret == RT_FOUND)
	{
		print_colour(guess, GREEN, WORD_LEN);
		return ;
	}
	guess.type = check_guess(target, pos, guess.ch);
}

void	init_ctx(t_ctx *c)
{
	memset(c, 0, sizeof(t_ctx));
	read_dict(c);
	select_target(c);
}

int	main()
{
	t_ctx	c;

	init_ctx(&c);
	while (++guess_cnt <= 5)
		handle_user_input(&c);
}
