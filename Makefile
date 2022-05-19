NC = \033[0m
RED = \033[1;31m
GREEN = \033[1;32m
ORANGE = \033[1;33m

COMPILER = clang++
FLAGS = -Wall -Wextra -Werror -std=c++98

NAME = webserv

INCLUDES = webserv.hpp \
			parser/Parser.hpp \
			parser/Tokenizer.hpp \
			parser/Utils.hpp

SRCS = main.cpp \
		parser/Parser.cpp \
		parser/Tokenizer.cpp \
		parser/Utils.cpp

all: $(NAME)

$(NAME): $(INCLUDES) $(SRCS)
	@$(COMPILER) $(FLAGS) $(SRCS) -o $(NAME)
	@echo "$(GREEN)Compilation successful$(NC)"

clean:
	@echo "$(GREEN)Clean successful$(NC)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(GREEN)Full clean successful$(NC)"

re: fclean all

.PHONY: all clean fclean re
