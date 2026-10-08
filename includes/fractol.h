/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbraune <fbraune@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/03 19:10:18 by fbraune           #+#    #+#             */
/*   Updated: 2026/10/08 12:09:30 by fbraune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H

# include "../Libft/libft.h"
# include "../mlx42/include/MLX42/MLX42.h"
# include "colors.h"
# include <stdbool.h>
# include <unistd.h>

# define WIN_WIDTH 800
# define WIN_HEIGHT 800
# define MAX_ITER 2000
# define START_ITER 100
# define REFINE_STEP 5
# define REFINE_DELAY 0.03
# define MOVE_STEP 20
# define MOVE_SPEED 400.0
# define ZOOM_FACTOR 1.1
# define ZOOM_MIN 0.1
# define AUTO_COLOR_SPEED 10.0
# define SCROLL_SETTLE_DELAY 0.05
# define MANDELBROT_CENTER_X -0.75
# define MANDELBROT_CENTER_Y 0.0
# define COLOR_SHIFT_WRAP 100.0
# define INSIDE_COLOR 0x000000FF
# define BACKGROUND_COLOR 0x1A1A24FF

# define USAGE "Usage: ./fractol Mandelbrot | ./fractol Sierpinski | \
./fractol Julia <real> <imaginary>"

typedef enum s_fractol_type
{
	MANDELBROT,
	JULIA,
	SIERPINSKI
}					t_fratol_type;

typedef struct s_fractol
{
	mlx_t			*mlx;
	mlx_image_t		*img;
	t_fratol_type	type;
	double			julia_x;
	double			julia_y;
	double			zoom;
	int				mouse_x;
	int				mouse_y;
	double			scroll_accum;
	double			scroll_anchor_x;
	double			scroll_anchor_y;
	double			scroll_timer;
	bool			scroll_pending;
	double			refine_timer;
	double			color_shift;
	bool			auto_color;
	bool			needs_resize;
	int				max_iter;
	int				iter;
	int				win_width;
	int				win_height;
	double			offset_x;
	double			offset_y;
}					t_fractol;

typedef struct s_render
{
	t_fractol		*fractol;
	int				width;
	int				height;
	double			center_x;
	double			center_y;
	double			scale;
	double			z[2];
	double			j[2];
	double			cr;
	double			ci;
	int				iter;
	int				x;
	int				y;
}					t_render;

void				write_error(t_fractol *fractol, char *msg);
double				parse_double(const char *value);
void				init_values(t_fractol *fractol);
void				set_values(int argc, const char **argv,
						t_fractol *fractol);
void				apply_zoom(t_fractol *fractol, double factor,
						double anchor_x, double anchor_y);
void				set_center(t_fractol *fractol, double *center_x,
						double *center_y);
void				key_hook(mlx_key_data_t keydata, void *param);
void				close_hook(void *param);
void				scroll_hook(double xdelta, double ydelta, void *param);
void				cursor_hook(double xpos, double ypos, void *param);
void				loop_hook(void *param);
void				resize_hook(int32_t width, int32_t height, void *param);
void				move_helper(t_fractol *fractol, double step, bool *moved);
void				handle_zoom(mlx_key_data_t keydata, t_fractol *fractol);
void				color_shift(mlx_key_data_t keydata, t_fractol *fractol);
double				get_dt(const t_fractol *fractol);
int					mandelbrot_iter(double cr, double ci, int max_iter);
int					julia_iter(double z[2], const double j[2], int max_iter);
int					sierpinski_iter(double x, double y, int max_iter);
uint32_t			get_color(int iter, int cur_iter, int max_iter,
						double shift);
void				render_fractal(t_fractol *fractol);

#endif