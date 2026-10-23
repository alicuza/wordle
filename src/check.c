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
