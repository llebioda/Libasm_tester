TMP_DIR = tmp

LIBASM_DIR = ..
LIBASM = $(LIBASM_DIR)/libasm.a
LIBASM_LIB = -L$(LIBASM_DIR) -lasm

BONUS_TARGETS_ARGS := \
	atoi_base \
	list_push_front \
	list_size \
	list_sort \
	list_remove_if

TARGETS_ARGS := $(BONUS_TARGETS_ARGS) \
	strlen \
	strcpy \
	strcmp \
	write \
	read \
	strdup

ARGS := $(filter $(TARGETS_ARGS), $(MAKECMDGOALS))

CC = cc
CFLAGS = -Wall -Wextra -Werror -g3 -D TMP_DIR='"$(TMP_DIR)"'

# Iterate over each target and check if the corresponding file exists
# If it doesnt exist add a DEFINE to disable that part to avoid error during compilation
define CHECK_FILE
ifneq ($(wildcard $(LIBASM_DIR)/ft_$(1)_bonus.s), $(LIBASM_DIR)/ft_$(1)_bonus.s)
    CFLAGS += -D disable_$(1)
endif
endef

$(foreach target, $(BONUS_TARGETS_ARGS), $(eval $(call CHECK_FILE,$(target))))

SRCS = main.c utils.c $(addsuffix _test.c, $(TARGETS_ARGS))

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

vtest: libasm $(NAME) $(TMP_DIR)
	@echo -n "$(GREEN_COLOR)Executing with valgrind ./$(NAME)$(RESET_COLOR)\n"
	@valgrind -q --show-leak-kinds=all --leak-check=full --track-origins=yes ./$(NAME) $(ARGS)

$(NAME): $(HEADERS) $(OBJS) $(LIBASM)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) $(LIBASM_LIB) -o $(NAME)
	@echo -n "$(GREEN_COLOR)Compilation successfull$(RESET_COLOR)\n"

$(OBJDIR)%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -MF $(@:.o=.d) -c $< -o $@

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(TMP_DIR):
	@mkdir -p $(TMP_DIR)

libasm:
	@make -C $(LIBASM_DIR) bonus --no-print-directory || make -C $(LIBASM_DIR) --no-print-directory

clean:
	rm -f $(OBJS) $(DEPS)
	rm -rf $(TMP_DIR)

fclean: clean
	rm -f $(NAME)
	@make -C $(LIBASM_DIR) fclean --no-print-directory

re: fclean
	@$(MAKE) --no-print-directory test

-include $(DEPS)

.PHONY: test vtest libasm clean fclean re

%:
	@: