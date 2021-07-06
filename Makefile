NAME = minishell

CC = gcc

CFLAGS = -Wall -Wextra -Werror -I includes/ -I libft/

LIBFT = -L. -lft -lreadline

PARSER_DIR = parser/

PARSER_LIST = parse.c \

UTILS_DIR = parser/utils/

UTILS_LIST = string_utils.c \

GNL_DIR = gnl/

GNL_LIST = get_next_line.c \
		get_next_line_utils.c \

SRC_DIR = src/

SRC_LIST = minishell.c \
			src_utils.c \
			envp.c \
			signals.c \
			redirect.c \
			bin.c \
			search_utils.c \
			increment_lvl.c \
			built-in/unset.c \
			built-in/cd.c \
			built-in/export.c \
			built-in/pwd.c \
			built-in/exit.c \
			built-in/echo.c \
			built-in/env.c \


SRC = $(addprefix $(SRC_DIR), $(SRC_LIST))

GNL = $(addprefix $(GNL_DIR), $(GNL_LIST))

PARSER = $(addprefix $(PARSER_DIR), $(PARSER_LIST))

UTILS = $(addprefix $(UTILS_DIR), $(UTILS_LIST))

SRCS = $(SRC) $(GNL) $(UTILS) $(PARSER)

OBJ = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJ) libft
	@echo "\n"
	@echo "\033[0;32mCompiling minishell..."
	@make -C libft/
	cp libft/libft.a .
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT)
	@echo "\n\033[0mDone !"

%.o: %.c
	@printf "\033[0;33mGenerating minishell objects... %-33.33s\r" $@
	@${CC} -g ${CFLAGS} -c $< -o $@

clean:
	@echo "\033[0;31mCleaning libft..."
	@make clean -C libft/
	@echo "\nRemoving binaries..."
	@rm -f $(OBJ)
	@echo "\033[0m"

fclean:
	@echo "\033[0;31mCleaning libft..."
	@make fclean -C libft/
	@echo "\nDeleting objects..."
	@rm -f $(OBJ)
	@echo "\nDeleting executable..."
	@rm -f $(NAME)
	@echo "\033[0m"

re: fclean all

test: all
	./$(NAME)

norm:
	norminette $(SRC) includes/$(HEADER)

.PHONY: clean fclean re test norm
