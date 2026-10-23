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
CPPFLAGS = -I$(INC_DIR)
LDLIBS = -lreadline

SRC_DIR = src
INC_DIR = inc
OBJ_DIR = obj

SRCS = main.c init.c input.c check.c print.c
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))
DEPS = $(OBJS:.o=.d)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR)

fclean:
	$(MAKE) clean
	rm -f $(NAME)

re:
	$(MAKE) fclean
	$(MAKE) all

.PHONY: all clean fclean re
-include $(DEPS)
