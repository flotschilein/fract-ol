NAME = fractol
CC = cc
CFLAGS = -Wall -Wextra -Werror

SRC_DIR = src/
LIBFT_DIR = Libft/
MLX42_DIR = mlx42/
MLX42_REPO = https://github.com/codam-coding-college/MLX42.git
INCLUDES = -I includes/ -I $(LIBFT_DIR) -I $(MLX42_DIR)include/

SRC = $(SRC_DIR)main.c

OBJ = $(SRC:.c=.o)
LIBFT = $(LIBFT_DIR)libft.a
MLX42_LIB = $(MLX42_DIR)build/libmlx42.a

ifeq ($(OS),Windows_NT)
    LIBS = -lglfw3 -lopengl32 -lgdi32
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Darwin)
        LIBS = -lglfw -framework Cocoa -framework OpenGL -framework IOKit
    endif
    ifeq ($(UNAME_S),Linux)
        LIBS = -lglfw -lGL -ldl -lm -lpthread
    endif
endif

all: clone_mlx42 $(NAME)

$(NAME): $(OBJ) $(LIBFT) $(MLX42_LIB)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(MLX42_LIB) -o $(NAME) $(LIBS)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(MLX42_LIB):
	cmake -B $(MLX42_DIR)build -S $(MLX42_DIR)
	cmake --build $(MLX42_DIR)build

clone_mlx42:
	@if [ ! -d "$(MLX42_DIR)/.git" ]; then \
	   echo "Cloning MLX42 library..."; \
	   git clone $(MLX42_REPO) $(MLX42_DIR); \
	else \
	   echo "MLX42 already cloned."; \
	fi

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	make -C $(LIBFT_DIR) clean
	rm -rf $(OBJ) $(MLX42_DIR)build

fclean: clean
	make -C $(LIBFT_DIR) fclean
	rm -rf $(NAME)

re: fclean all

run: all
	./$(NAME) Mandelbrot

.PHONY: all bonus clean fclean re run clone_mlx42