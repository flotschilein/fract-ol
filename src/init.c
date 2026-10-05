/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbraune <fbraune@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 19:58:02 by fbraune           #+#    #+#             */
/*   Updated: 2026/10/05 20:18:54 by fbraune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fractol.h"

/**
 * @brief Sets every value of the state to its default.
 *
 * @param fractol The fractol state to fill.
 */
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

/**
 * @brief Reads the command line, then opens the window and makes the image.
 *
 * @param argc Number of arguments.
 * @param argv The arguments.
 * @param fractol The fractol state to fill.
 */
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