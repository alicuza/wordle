# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sancuta <sancuta@student.42vienna.com      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/10/22 09:31:48 by sancuta           #+#    #+#              #
#    Updated: 2026/10/22 12:25:23 by sancuta          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = wordle
CC = cc
CFLAGS = -Wall -Wextra -Werror -MD -MP
LDLIBS = -lreadline

SRCS = wordle.c
OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(LDLIBS) $^ -o $@

clean:
	rm -f $(OBJS)
	rm -f $(DEPS)

fclean:
	$(MAKE) clean
	rm -f $(NAME)

re:
	$(MAKE) fclean
	$(MAKE) all

.PHONY: all clean fclean re
-include $(DEPS)
