CC := gcc
# fPIC stands for `position independent code`
CFLAGS := -Wall -Wextra -Werror -nostdlib -fPIC -std=c99
NAME := libdynlib.so

SRCS := dynlib.c
OBJS := $(SRCS:.c=.o)

$(NAME): $(OBJS)
	$(CC) -shared -o $@ $^

%.o: %.c dynlib.h
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

.PHONY: clean
