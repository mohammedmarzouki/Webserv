NC = \033[0m
RED = \033[1;31m
GREEN = \033[1;32m
ORANGE = \033[1;33m

COMPILER = clang++
FLAGS = -Wall -Wextra -Werror -std=c++98 -g

NAME = webserv

INCLUDES = webserv.hpp \
			parser/Parser.hpp \
			parser/Tokenizer.hpp \
			handle_request/Handle_request.hpp \
			parser/Utils.hpp

SRCS = main.cpp \
		parser/Parser.cpp \
		parser/Tokenizer.cpp \
		handle_request/Handle_request.cpp \
		parser/Utils.cpp \
		tools/tools.cpp

all: $(NAME)

$(NAME): $(INCLUDES) $(SRCS)
	@$(COMPILER) $(FLAGS) $(SRCS) -o $(NAME)
	@echo "$(GREEN)Compilation successful$(NC)"

clean:
	@rm -rf $(NAME).dSYM .vscode
	@echo "$(GREEN)Clean successful$(NC)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(GREEN)Full clean successful$(NC)"

re: fclean all

.PHONY: all clean fclean re
