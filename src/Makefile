CC := gcc
CFLAGS := -Wall -Wextra -Werror -std=c99
LDFLAGS := -ldl
NAME := dlopen
NAME_NOLIBS := dlopen_nolibs
LIB_DIRS := dynlib
LIB_NAMES := dynlib
LIB_FILES := $(foreach lib,$(LIB_NAMES),$(lib)/lib$(lib).so)
SRCS := main.c
OBJS := $(SRCS:.c=.o)

all: libs $(NAME)

.PHONY: libs
libs: $(LIB_FILES)

dynlib/libdynlib.so:
	$(MAKE) -C dynlib

# newlib/libnewlib.so:
#	$(MAKE) -C newlib

$(NAME): $(OBJS) $(LIB_FILES)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LDFLAGS)

# Kütüphanesiz executable
.PHONY: nolibs
nolibs: $(NAME_NOLIBS)

$(NAME_NOLIBS): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME_NOLIBS) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY: clean
clean: clean-dynlib
	$(RM) $(OBJS)

.PHONY: clean-dynlib
clean-dynlib:
	$(MAKE) -C dynlib clean

# .PHONY: clean-newlib
# clean-newlib:
#	$(MAKE) -C newlib clean

.PHONY: fclean
fclean: fclean-dynlib
	$(RM) $(NAME) $(NAME_NOLIBS) $(OBJS)

.PHONY: fclean-dynlib
fclean-dynlib:
	$(MAKE) -C dynlib fclean

# .PHONY: fclean-newlib
# fclean-newlib:
#	$(MAKE) -C newlib fclean

.PHONY: re
re: clean all

.PHONY: all
