/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wordle.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 19:54:10 by sancuta           #+#    #+#             */
/*   Updated: 2026/10/22 00:29:25 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORDLE_H
# define WORDLE_H

# include <unistd.h>	// read, write, close
# include <sys/stat.h>	// stat
# include <fcntl.h>		// open
# include <time.h>
# include <errno.h>
# include <stdint.h>

# define MAX_GUESS 6
# define WORD_LEN 5
# define WORD_CNT 12972
# define SEED 65189
# define PATH "./wordle.txt"
# define PROMPT "Enter a 5 letter word: "

/*=== colours ======*/
# define RESET	\033[0m
# define GREEN	\033[38;5;2m
# define YELLOW	\033[38;5;3m

typedef enum e_match_type
{
	MT_NO_MATCH,	//grey
	MT_WRONG_POS,	//yellow
	MT_MATCH,		//green
	MT_COUNT
}	t_match_type;

typedef enum e_return_type
{
	RT_NOT_FOUND,
	RT_FOUND,
	RT_TOO_SHORT,
	RT_COUNT
}	t_return_type;

typedef struct s_ctx
{
	char			*dict[WORD_LEN];
	uint32			target_idx;
	char			guess[WORD_LEN];
	t_match_type	type[WORD_LEN];
	uint8			guess_cnt;
	
}	t_ctx;



#endif
