NAME	= r-type

CC	= g++

RM	= rm -f

SRCS	= ./source/main.cpp \
    ./source/lib/utils.cpp

OBJS	= $(SRCS:.cpp=.o)

CPPFLAGS = -I ./source/includes/ -lsfml-graphics -lsfml-window -lsfml-system
CPPFLAGS += -Wall -Wextra -lsfml-graphics -lsfml-window -lsfml-system

all: $(NAME)

$(NAME): $(OBJS)
	 $(CC) $(OBJS) -o $(NAME) $(LDFLAGS)  -lsfml-graphics -lsfml-window -lsfml-system

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
