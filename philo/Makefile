# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/27 04:38:53 by tafujise          #+#    #+#              #
#    Updated: 2026/02/27 04:45:39 by tafujise         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME:=philo

SRCS:=main.c\

HEADERS:=philo.h\

OBJDIR:=obj
OBJS:=$(addprefix $(OBJDIR)/,$(SRCS:%.c=%.o))

CC:=cc

CCFLAGS:=-Wall -Wextra -Werror

DEBUG_FLAGS:=-g -O0

all:$(NAME)

$(NAME): $(OBJS)
	$(CC) -pthread -o $(NAME) $(OBJS)

$(OBJDIR)/%.o: %.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

debug: CCFLAGS += $(DEBUG_FLAGS)
debug: re

.PHONY: all clean fclean re debug
