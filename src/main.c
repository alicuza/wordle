#include "wordle.h"

int	main(void)
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
