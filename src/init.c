#include "wordle.h"

static bool	get_word(int fd, char *input)
{
	ssize_t	read_len;

	errno = 0;
	read_len = read(fd, input, WORD_LEN + 1);
	while (read_len == -1)
	{
		if (errno != EINTR)
		{
			perror("get_word");
			exit(EXIT_FAILURE);
		}
		errno = 0;
		read_len = read(fd, input, WORD_LEN + 1);
	}
	if (read_len <= 0)
		return (false);
	input[WORD_LEN] = '\0';
	return (true);
}

static void	read_dict(t_ctx *c)
{
	int			fd;
	uint32_t	i;

	errno = 0;
	fd = open(PATH, O_RDONLY);
	while (fd == -1)
	{
		if (errno == EINTR)
		{
			fd = open(PATH, O_RDONLY);
			errno = 0;
		}
		else
		{
			perror("read_dict");
			exit(EXIT_FAILURE);
		}
	}
	i = -1;
	while (++i < WORD_CNT && get_word(fd, c->dict[i]))
		;
	close(fd);
}

static uint32_t	randomize(uint32_t x)
{
	x ^= x >> 16;
	x *= 0x85ebca6b;
	x ^= x >> 13;
	x *= 0xc2b2ae35;
	x ^= x >> 16;
	return (x);
}

static void	select_target(t_ctx *c)
{
	struct tm	tm;
	time_t		t;
	uint32_t	day;

	t = time(NULL);
	tm = *localtime(&t);
	day = tm.tm_year * 366 + tm.tm_yday;
	c->target_idx = randomize(day ^ SEED) % WORD_CNT;
}

void	init_ctx(t_ctx *c)
{
	uint8_t	i;

	memset(c, 0, sizeof(t_ctx));
	i = -1;
	while (++i < MAX_GUESS)
	{
		memset(c->guess[i], '_', WORD_LEN);
		c->guess[i][WORD_LEN] = '\0';
	}
	c->guess_cnt = -1;
	read_dict(c);
	select_target(c);
}
