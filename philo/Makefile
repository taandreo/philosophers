.PHONY: all clean fclean re log bonus

CC = cc
NAME = philo
# BONUS = checker
CFLAGS = -Wall -Wextra -Werror
UNAME := $(shell uname)
LIBS := -lpthread

ifeq ($(UNAME), Darwin)
	CFLAGS = -Wall -Wextra -Werror -arch x86_64
endif

SRCS = philosophers.c utils.c time.c actions.c args.c
SRCS_MAIN  =  $(SRCS)
# SRCS_BONUS = checker.c $(SRCS)

OBJS_MAIN  = $(SRCS_MAIN:.c=.o)
# OBJS_BONUS = $(SRCS_BONUS:.c=.o)

all: $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -I include -c $< -o $@ 

$(NAME): $(OBJS_MAIN)
	$(CC) $(CFLAGS) $(OBJS_MAIN) -o $(NAME) $(LIBS)

# bonus: $(OBJS_BONUS) | libft
# 	$(CC) $(CFLAGS) $(OBJS_BONUS) -o $(BONUS) $(LIBS) -L $(LIBFT_DIR)

clean:
	rm -f $(OBJS_MAIN) $(OBJS_BONUS)

fclean: clean
	rm -f $(NAME) $(BONUS) $(LIBFT) 

re: fclean all
