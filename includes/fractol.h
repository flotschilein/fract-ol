/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbraune <fbraune@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/03 19:10:18 by fbraune           #+#    #+#             */
/*   Updated: 2026/10/03 21:30:32 by fbraune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H

# include <stdbool.h>
# include <unistd.h>

# include "colors.h"
# include "../mlx42/include/MLX42/MLX42.h"
# include "../Libft/libft.h"

# define WIN_WIDTH 800
# define WIN_HEIGHT 800
# define MAX_ITER 100
# define MOVE_STEP 20
# define ZOOM_FACTOR 1.1
# define ZOOM_MIN 0.1
# define ZOOM_MAX 1e6
# define AUTO_COLOR_EVERY_N_FRAMES 10

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
    bool auto_color;
    int iterations;
    int max_iter;
    int win_width;
    int win_height;
    double offset_x;
    double offset_y;
} t_fractol;

void    write_error(char *msg);
double  parse_double(const char *value);
void    init_values(t_fractol *fractol);
void    debug_print(t_fractol *fractol);
void    apply_zoom(t_fractol *fractol, double factor, double anchor_x, double anchor_y);
void    key_hook(mlx_key_data_t keydata, void *param);
void    close_hook(void *param);
void    scroll_hook(double xdelta, double ydelta, void *param);
void    cursor_hook(double xpos, double ypos, void *param);
void    loop_hook(void *param);
void    resize_hook(int32_t width, int32_t height, void *param);
int     mandelbrot_iter(double cr, double ci, int max_iter);
uint32_t get_color_simple(int iter, int max_iter);
void    render_fractal(t_fractol *fractol);

#endif