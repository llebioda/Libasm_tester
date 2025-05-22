TMP_DIR = tmp

LIBASM_DIR = ../
LIBASM = $(LIBASM_DIR)libasm.a

TARGETS_ARGS := strlen \
				strcpy \
				strcmp \
				write \
				read \
				strdup \
				atoi_base \
				list_push_front \
				list_size \
				list_sort \
				list_remove_if

ARGS := $(filter $(TARGETS_ARGS), $(MAKECMDGOALS))

CC = cc
CFLAGS = -Wall -Wextra -Werror -g -D $(TMP_DIR)

SRCS =	main.c \
		strlen_test.c \
		strcpy_test.c \
		strcmp_test.c \
		write_test.c \
		read_test.c
#$(addsuffix _test.c, $(TARGETS_ARGS))

OBJDIR = ./build/
OBJS = $(addprefix $(OBJDIR), $(notdir $(SRCS:.c=.o)))
DEPS = $(OBJS:.o=.d)

HEADERS = libasm_tester.h
INCLUDES = -I.

NAME = tester

RESET_COLOR = \033[m
GREEN_COLOR = \033[0;32m

test: libasm $(NAME) $(TMP_DIR)
	@echo -n "$(GREEN_COLOR)Executing ./$(NAME)$(RESET_COLOR)\n"
	@./$(NAME) $(ARGS)

$(NAME): $(HEADERS) $(OBJS) $(LIBASM)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) $(LIBASM) -o $(NAME)
	@echo -n "$(GREEN_COLOR)Compilation successfull$(RESET_COLOR)\n"

$(OBJDIR)%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -MF $(@:.o=.d) -c $< -o $@

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(TMP_DIR):
	@mkdir -p $(TMP_DIR)

libasm:
	@make -C $(LIBASM_DIR) --no-print-directory

clean:
	rm -f $(OBJS) $(DEPS)
	rm -rf $(TMP_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean
	@$(MAKE) --no-print-directory test

-include $(DEPS)

.PHONY: test libasm clean fclean re

%:
	@: