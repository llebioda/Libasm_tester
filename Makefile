CC = cc
CFLAGS = -Wall -Wextra -Werror -g

LIBASM_DIR = ../
LIBASM = $(LIBASM_DIR)libasm.a

SRCS =	main.c \
		strlen_test.c \
		strcpy_test.c \
		strcmp_test.c

OBJDIR = ./build/
OBJS = $(addprefix $(OBJDIR), $(notdir $(SRCS:.c=.o)))
DEPS = $(OBJS:.o=.d)

HEADERS = libasm_tester.h
INCLUDES = -I.

NAME = tester

RESET_COLOR = \033[m
GREEN_COLOR = \033[0;32m

test: libasm $(NAME)
	@echo -n "$(GREEN_COLOR)Executing ./$(NAME)$(RESET_COLOR)\n"
	@./$(NAME)

$(NAME): $(HEADERS) $(OBJS) $(LIBASM)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) $(LIBASM) -o $(NAME)
	@echo -n "$(GREEN_COLOR)Compilation successfull$(RESET_COLOR)\n"

$(OBJDIR)%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -MF $(@:.o=.d) -c $< -o $@

$(OBJDIR):
	@mkdir -p $(OBJDIR)

libasm:
	@make -C $(LIBASM_DIR) --no-print-directory

clean:
	rm -f $(OBJS) $(DEPS)

fclean: clean
	rm -f $(NAME)

re: fclean
	@$(MAKE) --no-print-directory test

-include $(DEPS)

.PHONY: test libasm clean fclean re