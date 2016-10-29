NAME		=	mod1
SDL2I		=	$(shell sdl2-config --cflags)
SDL2L		=	$(shell sdl2-config --libs)
INC			=	-I includes $(SDL2I)
LIBS		=	-lm $(SDL2L)
CFLAGS		=	-Wall -Wextra -Werror $(INC)
LFLAGS		=	-lpthread $(INC) $(LIBS)
CC			=	g++ -std=c++11 -g3 
LD			=	g++ -std=c++11 -g3 

INCLUDE		=	$(shell find includes -name "*.h")
SRC			=	$(shell find src -name "*.cpp")
OBJ			=	$(SRC:src/%.c=build/%.o)

all: mod1

build/%.o: src/%.c $(INCLUDE)
		@echo "\033[92m    CC    $@\033[0m"
			@mkdir -p $(dir $@)
				$(CC) $(CFLAGS) -c $< -o $@

mod1: $(OBJ)
		@echo "\033[92m    LD    $(NAME)\033[0m"
		@echo "$(SDL2I)";
		@echo "$(SDL2L)";
		@$(LD) $^ $(LFLAGS) -o $(NAME)

clean:
		@echo "\033[92m    RM    object files\033[0m"
			@rm -rf build

fclean: clean
		@echo "\033[92m    RM    $(NAME)\033[0m"
			@rm -f $(NAME)
				@rm -rf $(NAME).dSYM

re: fclean all
