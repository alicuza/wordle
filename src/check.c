/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kseltenr <kseltenr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/23 20:44:27 by kseltenr          #+#    #+#             */
/*   Updated: 2026/10/23 20:44:59 by kseltenr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wordle.h"

/* Marks letters in the right spot green. Returns a bitmask of the
 * target letters that are still unused. */
static uint8_t	mark_exact(char *guess, char *target, t_match_type *mt)
{
	uint8_t	i;
	uint8_t	guess_mask;

	guess_mask = 63;	// 0b00111111
	i = -1;
	while (++i < WORD_LEN)
	{
		if (target[i] == tolower(guess[i]))
		{
			mt[i] = MT_MATCH;
			guess_mask ^= 1 << i;
		}
	}
	return (guess_mask);
}

/* Marks letters that are in the word but in the wrong spot yellow. */
static void	mark_wrong_pos(char *guess, char *target, t_match_type *mt,
	uint8_t guess_mask)
{
	uint8_t	i;
	uint8_t	j;
	uint8_t	current_bit;

	i = -1;
	while (++i < WORD_LEN)
	{
		if (mt[i] == MT_MATCH)
			continue ;
		j = -1;
		while (++j < WORD_LEN)
		{
			current_bit = 1 << j;
			if (target[j] == tolower(guess[i]) && guess_mask & current_bit)
			{
				mt[i] = MT_WRONG_POS;
				guess_mask ^= current_bit;
				break ;
			}
		}
	}
}

/* Upgrades each guessed letter's keyboard state; never downgrades
 * (e.g. a green letter stays green even if it's grey elsewhere). */
static void	update_keys(t_ctx *c)
{
	uint8_t		i;
	uint8_t		letter;
	t_key_state	state;

	i = -1;
	while (++i < WORD_LEN)
	{
		letter = tolower(c->guess[c->guess_cnt][i]) - 'a';
		if (letter >= ALPHA_LEN)
			continue ;
		state = (t_key_state)(c->type[c->guess_cnt][i] + 1);
		if (state > c->keys[letter])
			c->keys[letter] = state;
	}
}

void	check_guess(t_ctx *c)
{
	char			*guess;
	char			*target;
	t_match_type	*mt;
	uint8_t			guess_mask;

	guess = c->guess[c->guess_cnt];
	target = c->dict[c->target_idx];
	mt = c->type[c->guess_cnt];
	guess_mask = mark_exact(guess, target, mt);
	mark_wrong_pos(guess, target, mt, guess_mask);
	update_keys(c);
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
	if (!c->guess_cnt)
		write(1, VICTORY3a, strlen(VICTORY3a));
	else
		write(1, VICTORY3b, strlen(VICTORY3b));
	return (true);
}
