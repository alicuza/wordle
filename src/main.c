/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kseltenr <kseltenr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/23 20:44:00 by kseltenr          #+#    #+#             */
/*   Updated: 2026/10/23 20:44:04 by kseltenr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wordle.h"

int	main(void)
{
	t_ctx	c;

	init_ctx(&c);
	print_field_blanks(&c);
	print_keyboard(&c);
	while (++c.guess_cnt < MAX_GUESS)
	{
		handle_user_input(&c);
		print_field_guesses(&c);
		print_field_blanks(&c);
		print_keyboard(&c);
		if (check_victory(&c))
			return (0);
	}
	print_loss(&c);
	return (0);
}
