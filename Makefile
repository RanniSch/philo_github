# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rschlott <rschlott@student.42wolfsburg.de> +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/30 17:21:54 by rschlott          #+#    #+#              #
#    Updated: 2023/01/06 14:46:14 by rschlott         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ========= compilation ==============
# -g is for debugging info
NAME = philo
CC = gcc
CFLAGS = -Wall -Wextra -Werror #-g -O3 -fsanitize=address #-fsanitize=thread

RM			:= rm -f

HEADER = ./incl/philo.h

SRCS = 	./src/main.c \
		./src/errors.c \
		./src/input_check.c \
		./src/process.c \
		./src/time.c \
		./src/philosopher.c \
		./src/text_output.c \
		./src/multiple_killer.c

# takes all source files converts them into .o and puts them into /obj
OBJS = ${SRCS:.c=.o}

# ============== rules ================
# start of the makefile execution
# if all comes after $(NAME) recompile message changes to: "make: 'philo' is up to date."
all:	$(NAME)

# -pthread is for linking the pthread library
# compiles the object files and creates the executable name after NAME
$(NAME) : $(OBJS)
		$(CC) $(CFLAGS) $(SRCS) -pthread -o $(NAME)
		
#remove all object files
clean:
	$(RM) ${OBJS}

# clean + remove executable
fclean: clean
	$(RM) $(NAME)

# make the whole compilation from scratch
re: fclean all

# prevents rules and files with the same names potentially being mixed up
.PHONY: all clean fclean re