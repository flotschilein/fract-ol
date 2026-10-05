/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbraune <fbraune@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/03 19:09:24 by fbraune           #+#    #+#             */
/*   Updated: 2026/10/05 19:54:38 by fbraune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fractol.h"
#include <errno.h>
#include <math.h>

void	write_error(t_fractol *fractol, char *msg)
{
	write(2, BOLD_RED, ft_strlen(BOLD_RED));
	if (!msg)
		write(2, "FATAL ERROR", ft_strlen("FATAL ERROR"));
	else
		write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	write(2, RESET, ft_strlen(RESET));
	if (fractol && fractol->mlx)
	{
		mlx_terminate(fractol->mlx);
		fractol->mlx = NULL;
		fractol->img = NULL;
	}
	exit(1);
}

static int	parse_sign(const char *val, int *i)
{
	int	sign;

	sign = 1;
	while (val[*i] == ' ' || (val[*i] >= '\t' && val[*i] <= '\r'))
		(*i)++;
	if (val[*i] == '-' || val[*i] == '+')
	{
		if (val[*i] == '-')
			sign = -1;
		(*i)++;
	}
	return (sign);
}

static double	parse_integer(const char *val, int *i, int *has_digits)
{
	double	number;

	number = 0.0;
	while (val[*i] >= '0' && val[*i] <= '9')
	{
		number = number * 10.0 + (val[*i] - '0');
		*has_digits = 1;
		(*i)++;
	}
	return (number);
}

static double	parse_fraction(const char *val, int *i, double *divisor,
		int *has_digits)
{
	double	fraction;

	fraction = 0.0;
	*divisor = 1.0;
	if (val[*i] == '.')
	{
		(*i)++;
		while (val[*i] >= '0' && val[*i] <= '9')
		{
			fraction = fraction * 10.0 + (val[*i] - '0');
			*divisor *= 10.0;
			*has_digits = 1;
			(*i)++;
		}
	}
	return (fraction);
}

double	parse_double(const char *value)
{
	double	num;
	double	div;
	int		sign;
	int		i;
	int		has_digits;

	if (!value || value[0] == '\0')
		write_error(NULL, USAGE);
	i = 0;
	has_digits = 0;
	sign = parse_sign(value, &i);
	num = parse_integer(value, &i, &has_digits);
	num += parse_fraction(value, &i, &div, &has_digits) / div;
	num *= sign;
	if (!has_digits || value[i] != '\0' || !isfinite(num))
		write_error(NULL, USAGE);
	return (num);
}

void	init_values(t_fractol *fractol)
{
	fractol->mlx = NULL;
	fractol->img = NULL;
	fractol->type = MANDELBROT;
	fractol->julia_x = 0.0;
	fractol->julia_y = 0.0;
	fractol->win_width = WIN_WIDTH;
	fractol->win_height = WIN_HEIGHT;
	fractol->zoom = 1.0;
	fractol->offset_x = 0.0;
	fractol->offset_y = 0.0;
	fractol->max_iter = MAX_ITER;
	fractol->iter = START_ITER;
	fractol->color_shift = 0.0;
	fractol->auto_color = false;
	fractol->needs_resize = false;
	fractol->mouse_x = 0;
	fractol->mouse_y = 0;
	fractol->scroll_accum = 0.0;
	fractol->scroll_anchor_x = 0.0;
	fractol->scroll_anchor_y = 0.0;
	fractol->scroll_timer = 0.0;
	fractol->scroll_pending = false;
	fractol->refine_timer = 0.0;
}

void	apply_zoom(t_fractol *fractol, double factor, double anchor_x,
		double anchor_y)
{
	double	new_zoom;
	double	eff;
	double	cx;
	double	cy;

	new_zoom = fractol->zoom * factor;
	if (new_zoom < ZOOM_MIN)
		new_zoom = ZOOM_MIN;
	eff = new_zoom / fractol->zoom;
	cx = fractol->win_width / 2.0;
	cy = fractol->win_height / 2.0;
	fractol->offset_x = fractol->offset_x * eff + (anchor_x - cx) * (eff - 1.0);
	fractol->offset_y = fractol->offset_y * eff + (anchor_y - cy) * (eff - 1.0);
	fractol->zoom = new_zoom;
}

void	color_shift(mlx_key_data_t keydata, t_fractol *fractol)
{
	if (keydata.modifier & MLX_SHIFT)
		fractol->color_shift -= 1.0;
	else
		fractol->color_shift += 1.0;
	render_fractal(fractol);
}

void	handle_zoom(mlx_key_data_t keydata, t_fractol *fractol)
{
	if (keydata.key == MLX_KEY_KP_ADD || keydata.key == MLX_KEY_EQUAL)
	{
		apply_zoom(fractol, ZOOM_FACTOR, fractol->win_width / 2.0,
			fractol->win_height / 2.0);
		fractol->iter = START_ITER;
		render_fractal(fractol);
	}
	else if (keydata.key == MLX_KEY_KP_SUBTRACT || keydata.key == MLX_KEY_MINUS)
	{
		apply_zoom(fractol, 1.0 / ZOOM_FACTOR, fractol->win_width / 2.0,
			fractol->win_height / 2.0);
		fractol->iter = START_ITER;
		render_fractal(fractol);
	}
}

void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_fractol	*fractol;

	fractol = (t_fractol *)param;
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
	{
		mlx_close_window(fractol->mlx);
		return ;
	}
	if (keydata.action != MLX_PRESS && keydata.action != MLX_REPEAT)
		return ;
	handle_zoom(keydata, fractol);
	if (keydata.key == MLX_KEY_C)
		color_shift(keydata, fractol);
	else if (keydata.key == MLX_KEY_V && keydata.action == MLX_PRESS)
		fractol->auto_color = !fractol->auto_color;
}

void	iteration_helper(t_fractol *fractol, double dt)
{
	fractol->refine_timer += dt;
	if (fractol->refine_timer >= REFINE_DELAY)
	{
		fractol->refine_timer = 0.0;
		fractol->iter += REFINE_STEP;
		if (fractol->iter > fractol->max_iter)
			fractol->iter = fractol->max_iter;
		render_fractal(fractol);
	}
}

void	resize_helper(t_fractol *fractol, bool *moved)
{
	fractol->needs_resize = false;
	if (fractol->img)
	{
		if (!mlx_resize_image(fractol->img, (uint32_t)fractol->win_width,
				(uint32_t)fractol->win_height))
			write_error(fractol, "mlx_resize_image failed");
		*moved = true;
	}
}

void	scroll_helper(t_fractol *fractol, double dt, bool *moved)
{
	fractol->scroll_timer += dt;
	if (fractol->scroll_timer >= SCROLL_SETTLE_DELAY)
	{
		apply_zoom(fractol, pow(ZOOM_FACTOR, fractol->scroll_accum),
			fractol->scroll_anchor_x, fractol->scroll_anchor_y);
		fractol->scroll_accum = 0.0;
		fractol->scroll_timer = 0.0;
		fractol->scroll_pending = false;
		*moved = true;
	}
}

void	move_helper(t_fractol *fractol, double step, bool *moved)
{
	if (mlx_is_key_down(fractol->mlx, MLX_KEY_UP)
		|| mlx_is_key_down(fractol->mlx, MLX_KEY_W))
	{
		fractol->offset_y += step;
		*moved = true;
	}
	if (mlx_is_key_down(fractol->mlx, MLX_KEY_DOWN)
		|| mlx_is_key_down(fractol->mlx, MLX_KEY_S))
	{
		fractol->offset_y -= step;
		*moved = true;
	}
	if (mlx_is_key_down(fractol->mlx, MLX_KEY_LEFT)
		|| mlx_is_key_down(fractol->mlx, MLX_KEY_A))
	{
		fractol->offset_x -= step;
		*moved = true;
	}
	if (mlx_is_key_down(fractol->mlx, MLX_KEY_RIGHT)
		|| mlx_is_key_down(fractol->mlx, MLX_KEY_D))
	{
		fractol->offset_x += step;
		*moved = true;
	}
}

void	color_helper(t_fractol *fractol, double dt, bool *redraw)
{
	fractol->color_shift += dt * AUTO_COLOR_SPEED;
	fractol->color_shift = fmod(fractol->color_shift, COLOR_SHIFT_WRAP);
	*redraw = true;
}

void	render_helper(t_fractol *fractol)
{
	fractol->iter = START_ITER;
	render_fractal(fractol);
}

double	get_dt(t_fractol *fractol)
{
	double	dt;

	dt = fractol->mlx->delta_time;
	if (dt < 0.0)
		dt = 0.0;
	if (dt > 0.05)
		dt = 0.05;
	return (dt);
}

void	loop_hook(void *param)
{
	t_fractol	*fractol;
	double		dt;
	double		step;
	bool		moved;
	bool		redraw;

	fractol = (t_fractol *)param;
	dt = get_dt(fractol);
	step = MOVE_SPEED * dt;
	moved = false;
	redraw = false;
	move_helper(fractol, step, &moved);
	if (fractol->scroll_pending)
		scroll_helper(fractol, dt, &moved);
	if (fractol->needs_resize)
		resize_helper(fractol, &moved);
	if (fractol->auto_color)
		color_helper(fractol, dt, &redraw);
	if (moved)
		render_helper(fractol);
	if (redraw)
		render_fractal(fractol);
	if (fractol->iter < fractol->max_iter)
		iteration_helper(fractol, dt);
}

void	close_hook(void *param)
{
	t_fractol	*fractol;

	fractol = (t_fractol *)param;
	mlx_close_window(fractol->mlx);
}

void	scroll_hook(double xdelta, double ydelta, void *param)
{
	t_fractol	*fractol;

	(void)xdelta;
	fractol = (t_fractol *)param;
	if (ydelta == 0)
		return ;
	mlx_get_mouse_pos(fractol->mlx, &fractol->mouse_x, &fractol->mouse_y);
	fractol->scroll_accum += ydelta;
	fractol->scroll_anchor_x = (double)fractol->mouse_x;
	fractol->scroll_anchor_y = (double)(fractol->win_height - fractol->mouse_y);
	fractol->scroll_timer = 0.0;
	fractol->scroll_pending = true;
}

void	cursor_hook(double xpos, double ypos, void *param)
{
	t_fractol	*fractol;

	fractol = (t_fractol *)param;
	fractol->mouse_x = (int)xpos;
	fractol->mouse_y = (int)ypos;
}

void	resize_hook(int32_t width, int32_t height, void *param)
{
	t_fractol	*fractol;

	fractol = (t_fractol *)param;
	if (width <= 0 || height <= 0)
		return ;
	if (width == fractol->win_width && height == fractol->win_height
		&& !fractol->needs_resize)
		return ;
	fractol->win_width = width;
	fractol->win_height = height;
	fractol->needs_resize = true;
}

int	mandelbrot_iter(double cr, double ci, int max_iter)
{
	double	zr;
	double	zi;
	double	tmp;
	int		i;

	zr = 0.0;
	zi = 0.0;
	i = 0;
	while (i < max_iter && (zr * zr + zi * zi) <= 4.0)
	{
		tmp = zr * zr - zi * zi + cr;
		zi = 2.0 * zr * zi + ci;
		zr = tmp;
		i++;
	}
	return (i);
}

int	julia_iter(double z[2], double j[2], int max_iter)
{
	double	zr;
	double	zi;
	double	tmp;
	int		i;

	zr = z[0];
	zi = z[1];
	i = 0;
	while (i < max_iter && (zr * zr + zi * zi) <= 4.0)
	{
		tmp = zr * zr - zi * zi + j[0];
		zi = 2.0 * zr * zi + j[1];
		zr = tmp;
		i++;
	}
	z[0] = zr;
	z[1] = zi;
	return (i);
}

bool	sierpinski_helper(int i, double *u, double *v, int *value)
{
	if (*u + *v <= 0.5)
	{
		*u *= 2.0;
		*v *= 2.0;
	}
	else if (*u >= 0.5)
	{
		*u = *u * 2.0 - 1.0;
		*v *= 2.0;
	}
	else if (*v >= 0.5)
	{
		*u *= 2.0;
		*v = *v * 2.0 - 1.0;
	}
	else
	{
		*value = (i);
		return (true);
	}
	return (false);
}

int	sierpinski_iter(double x, double y, int max_iter)
{
	int		i;
	double	u;
	double	v;
	int		value;

	u = (x + 1.0 - y) / 2.0;
	v = (1.0 - y - x) / 2.0;
	if (u < 0.0 || v < 0.0 || u + v > 1.0)
		return (-1);
	i = 0;
	while (i < max_iter)
	{
		if (sierpinski_helper(i, &u, &v, &value))
			return (value);
		i++;
	}
	return (i);
}

uint32_t	get_color(int iter, int cur_iter, int max_iter, double shift)
{
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;
	double	t;
	double	p;

	if (iter < 0)
		return (BACKGROUND_COLOR);
	if (iter >= cur_iter)
		return (INSIDE_COLOR);
	t = sqrt((double)iter / (double)max_iter);
	p = t + shift / 100.0;
	r = (uint8_t)(255.0 * (0.5 + 0.5 * sin(6.28318530718 * p)));
	g = (uint8_t)(255.0 * (0.5 + 0.5 * sin(6.28318530718 * (p + 0.33))));
	b = (uint8_t)(255.0 * (0.5 + 0.5 * sin(6.28318530718 * (p + 0.67))));
	return ((uint32_t)(r << 24 | g << 16 | b << 8 | 0xFF));
}

void	set_center(t_fractol *fractol, double *center_x, double *center_y)
{
	if (fractol->type == MANDELBROT)
	{
		*center_x = MANDELBROT_CENTER_X;
		*center_y = MANDELBROT_CENTER_Y;
	}
	else
	{
		*center_x = 0.0;
		*center_y = 0.0;
	}
}

static bool	init_render(t_fractol *fractol, t_render *render)
{
	render->fractol = fractol;
	render->width = (int)fractol->img->width;
	render->height = (int)fractol->img->height;
	if (render->width < 1 || render->height < 1)
		return (false);
	set_center(fractol, &render->center_x, &render->center_y);
	if (render->width < render->height)
		render->scale = 4.0 / (double)render->width;
	else
		render->scale = 4.0 / (double)render->height;
	render->scale /= fractol->zoom;
	render->j[0] = fractol->julia_x;
	render->j[1] = fractol->julia_y;
	return (true);
}

static void	set_coord(t_render *render)
{
	render->cr = render->center_x + ((double)render->x - render->width / 2.0
			+ render->fractol->offset_x) * render->scale;
	render->ci = render->center_y + ((double)(render->height - render->y)
			- render->height / 2.0 + render->fractol->offset_y) * render->scale;
}

static int	get_pixel_iter(t_render *render)
{
	render->z[0] = render->cr;
	render->z[1] = render->ci;
	if (render->fractol->type == JULIA)
		return (julia_iter(render->z, render->j, render->fractol->iter));
	if (render->fractol->type == SIERPINSKI)
		return (sierpinski_iter(render->cr, render->ci, render->fractol->iter));
	return (mandelbrot_iter(render->cr, render->ci, render->fractol->iter));
}

static void	draw_pixel(t_render *render)
{
	set_coord(render);
	render->iter = get_pixel_iter(render);
	mlx_put_pixel(render->fractol->img, (uint32_t)render->x,
		(uint32_t)render->y, get_color(render->iter, render->fractol->iter,
			render->fractol->max_iter, render->fractol->color_shift));
}

static void	render_row(t_render *render)
{
	render->x = 0;
	while (render->x < render->width)
	{
		draw_pixel(render);
		render->x++;
	}
}

void	render_fractal(t_fractol *fractol)
{
	t_render	render;

	if (!fractol || !fractol->img)
		return ;
	if (!init_render(fractol, &render))
		return ;
	render.y = 0;
	while (render.y < render.height)
	{
		render_row(&render);
		render.y++;
	}
}

void	set_values(int argc, const char **argv, t_fractol *fractol)
{
	init_values(&*fractol);
	if (argc == 2 && ft_strcmp((char *)argv[1], "Mandelbrot") == 0)
		fractol->type = MANDELBROT;
	else if (argc == 2 && ft_strcmp((char *)argv[1], "Sierpinski") == 0)
		fractol->type = SIERPINSKI;
	else if (argc == 4 && ft_strcmp((char *)argv[1], "Julia") == 0)
	{
		fractol->type = JULIA;
		fractol->julia_x = parse_double(argv[2]);
		fractol->julia_y = parse_double(argv[3]);
	}
	else
		write_error(fractol, USAGE);
	fractol->mlx = mlx_init(fractol->win_width, fractol->win_height, "fract-ol",
			true);
	if (!fractol->mlx)
		write_error(fractol, "mlx_init failed");
	fractol->img = mlx_new_image(fractol->mlx, (uint32_t)fractol->win_width,
			(uint32_t)fractol->win_height);
	if (!fractol->img)
		write_error(fractol, "mlx_new_image failed");
	if (mlx_image_to_window(fractol->mlx, fractol->img, 0, 0) < 0)
		write_error(fractol, "mlx_image_to_window failed");
}

int	main(int argc, const char **argv)
{
	t_fractol	fractol;

	set_values(argc, argv, &fractol);
	render_fractal(&fractol);
	mlx_resize_hook(fractol.mlx, resize_hook, &fractol);
	mlx_key_hook(fractol.mlx, key_hook, &fractol);
	mlx_close_hook(fractol.mlx, close_hook, &fractol);
	mlx_scroll_hook(fractol.mlx, scroll_hook, &fractol);
	mlx_cursor_hook(fractol.mlx, cursor_hook, &fractol);
	mlx_loop_hook(fractol.mlx, loop_hook, &fractol);
	mlx_loop(fractol.mlx);
	mlx_terminate(fractol.mlx);
	return (0);
}
