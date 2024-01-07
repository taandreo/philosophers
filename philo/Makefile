.PHONY: all clean fclean re log bonus

CC = cc
NAME = philo
CFLAGS = -Wall -Wextra -Werror
UNAME := $(shell uname)
LIBS := -lpthread

ifeq ($(UNAME), Darwin)
	CFLAGS = -Wall -Wextra -Werror -arch x86_64
endif

SRCS = philosophers.c utils0.c utils1.c actions.c init.c actions_utils.c
SRCS_MAIN  =  $(SRCS)

OBJS_MAIN  = $(SRCS_MAIN:.c=.o)

all: $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -I include -c $< -o $@ 

$(NAME): $(OBJS_MAIN)
	$(CC) $(CFLAGS) $(OBJS_MAIN) -o $(NAME) $(LIBS)

clean:
	rm -f $(OBJS_MAIN) $(OBJS_BONUS)

fclean: clean
	rm -f $(NAME) $(BONUS) $(LIBFT) 

re: fclean all
