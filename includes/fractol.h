/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbraune <fbraune@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/03 19:10:18 by fbraune           #+#    #+#             */
/*   Updated: 2026/10/03 20:07:14 by fbraune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H

# include <unistd.h>

# include "colors.h"
# include "../mlx42/include/MLX42/MLX42.h"
# include "../Libft/libft.h"

# define WIN_WIDTH 800
# define WIN_HEIGHT 800
# define MAX_ITER 100

typedef enum s_fractol_type {
    MANDELBROT,
    JULIA
} t_fratol_type;

typedef struct s_fractol {
    mlx_t *mlx;
    mlx_image_t *img;
    t_fratol_type type;
    double julia_x;
    double julia_y;
    double zoom;
    int mouse_x;
    int mouse_y;
    int color_shift;
    int iterations;
    int max_iter;
    int win_width;
    int win_height;
    int offset_x;
    int offset_y;
} t_fractol;

#endif