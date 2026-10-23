/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kseltenr <kseltenr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/23 20:43:51 by kseltenr          #+#    #+#             */
/*   Updated: 2026/10/23 20:43:56 by kseltenr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wordle.h"

static void	print_indent(size_t n)
{
	while (n--)
		write(1, " ", 1);
}

void	print_field_guesses(t_ctx *c)
{
	uint8_t	i;
	uint8_t	j;

	i = -1;
	while (++i <= c->guess_cnt)
	{
		print_indent(FIELD_INDENT);
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
	{
		print_indent(FIELD_INDENT);
		write(1, BLANK_FIELD, strlen(BLANK_FIELD));
	}
	write(1, "\n", 1);
}

void	print_loss(t_ctx *c)
{
	write(1, LOSS, strlen(LOSS));
	write(1, c->dict[c->target_idx], WORD_LEN);
	write(1, LOSS2, strlen(LOSS2));
}

static void	print_keyboard_row(t_ctx *c, char *row, uint8_t indent)
{
	t_key_state	state;

	print_indent(indent);
	while (*row)
	{
		state = c->keys[*row - 'a'];
		write(1, " ", 1);
		if (state == KS_MATCH)
			write(1, GREEN, strlen(GREEN));
		else if (state == KS_WRONG_POS)
			write(1, YELLOW, strlen(YELLOW));
		else if (state == KS_ABSENT)
			write(1, GREY, strlen(GREY));
		write(1, row, 1);
		write(1, RESET, strlen(RESET));
		row++;
	}
	write(1, "\n", 1);
}

void	print_keyboard(t_ctx *c)
{
	print_keyboard_row(c, KB_ROW1, 0);
	print_keyboard_row(c, KB_ROW2, 1);
	print_keyboard_row(c, KB_ROW3, 2);
	write(1, "\n", 1);
}
