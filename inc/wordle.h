/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wordle.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 19:54:10 by sancuta           #+#    #+#             */
/*   Updated: 2026/10/22 10:55:56 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORDLE_H
# define WORDLE_H

# include <unistd.h>			// read, write, close
# include <sys/stat.h>			// stat
# include <fcntl.h>				// open
# include <stdio.h>
# include <readline/readline.h>	// readline
# include <time.h>
# include <errno.h>
# include <stdint.h>
# include <stdbool.h>
# include <stdlib.h>
# include <string.h>
# include <ctype.h>			// tolower

# define MAX_GUESS 6
# define WORD_LEN 5
# define FIELD_LEN (3 * WORD_LEN)
# define WORD_CNT 12972
# ifndef SEED
#  define SEED 65537
# endif
# define PATH "./words.txt"
# define PROMPT "Enter a 5 letter word: "
# define WARN_TOO_SHORT "Too short. "
# define WARN_TOO_LONG "Too long. "
# define WARN_UNKNOWN_WORD "Word is unknown. "
# define VICTORY "You guessed "
# define VICTORY2 " in "
# define VICTORY3a " guess!\n"
# define VICTORY3b " guesses!\n"
# define LOSS "You lost. The word was "
# define LOSS2 ".\n"
# define BLANK_FIELD " _ _ _ _ _\n"
# define ALPHA_LEN 26
# define KB_ROW1 "qwertyuiop"
# define KB_ROW2 "asdfghjkl"
# define KB_ROW3 "zxcvbnm"
/* Spaces before each board row so it is centred over the keyboard. */
# define FIELD_INDENT (sizeof(KB_ROW1) - 1 - WORD_LEN)

/*=== colours ======*/
# define RESET	"\033[0m"
# define GREEN	"\033[38;5;2m"
# define YELLOW	"\033[38;5;3m"
# define GREY	"\033[38;5;8m"

typedef enum e_match_type
{
	MT_NO_MATCH,	//grey
	MT_WRONG_POS,	//yellow
	MT_MATCH,		//green
	MT_COUNT
}	t_match_type;

/* Best known state of a letter, shown on the keyboard. */
typedef enum e_key_state
{
	KS_UNUSED,		//default colour
	KS_ABSENT,		//grey
	KS_WRONG_POS,	//yellow
	KS_MATCH,		//green
	KS_COUNT
}	t_key_state;

typedef enum e_return_type
{
	RT_NOT_FOUND,
	RT_FOUND,
	RT_TOO_SHORT,
	RT_TOO_LONG,
	RT_COUNT
}	t_return_type;

typedef struct s_ctx
{
	uint32_t		target_idx;
	t_match_type	type[MAX_GUESS][WORD_LEN];
	char			dict[WORD_CNT][WORD_LEN + 1];
	char			guess[MAX_GUESS][WORD_LEN + 1];
	t_key_state		keys[ALPHA_LEN];
	uint8_t			guess_cnt;
	uint8_t			correct_char_cnt;
	bool			victory;
}	t_ctx;

/*=== init.c =======*/
void	init_ctx(t_ctx *c);

/*=== input.c ======*/
void	handle_user_input(t_ctx *c);

/*=== check.c ======*/
void	check_guess(t_ctx *c);
bool	check_victory(t_ctx *c);

/*=== print.c ======*/
void	print_field_guesses(t_ctx *c);
void	print_field_blanks(t_ctx *c);
void	print_loss(t_ctx *c);
void	print_keyboard(t_ctx *c);

#endif
