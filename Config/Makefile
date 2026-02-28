CC = c++
CFLAGS = -Wall -Wextra -Werror #-std=c++98
NAME = web_serv
SRCS = CgiHandler.cpp ConfigParser.cpp LocationConfig.cpp main.cpp router.cpp ServerConfig.cpp
HEADRS = CgiHandler.hpp ConfigParser.hpp httprequest.hpp LocationConfig.hpp router.hpp ServerConfig.hpp
OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

%.o: %.cpp $(HEADRS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re