#include "wordle.h"

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
