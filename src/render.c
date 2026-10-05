/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbraune <fbraune@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 20:08:09 by fbraune           #+#    #+#             */
/*   Updated: 2026/10/05 20:18:54 by fbraune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fractol.h"

/**
 * @brief Fills the render data and works out the scale.
 *
 * @param fractol The fractol state.
 * @param render The render data to fill.
 * @return false if the image has no size, true if it has one.
 */
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

/**
 * @brief Turns the current pixel into a point in the fractal.
 *
 * @param render The render data. cr and ci get the point.
 */
static void	set_coord(t_render *render)
{
	render->cr = render->center_x + ((double)render->x - render->width / 2.0
			+ render->fractol->offset_x) * render->scale;
	render->ci = render->center_y + ((double)(render->height - render->y)
			- render->height / 2.0 + render->fractol->offset_y) * render->scale;
}

/**
 * @brief Runs the chosen fractal on the current pixel.
 *
 * @param render The render data.
 * @return The number of steps, or -1 for a point outside the fractal.
 */
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

/**
 * @brief Works out the colour of the current pixel and writes it.
 *
 * @param render The render data.
 */
static void	draw_pixel(t_render *render)
{
	set_coord(render);
	render->iter = get_pixel_iter(render);
	mlx_put_pixel(render->fractol->img, (uint32_t)render->x,
		(uint32_t)render->y, get_color(render->iter, render->fractol->iter,
			render->fractol->max_iter, render->fractol->color_shift));
}

/**
 * @brief Draws every pixel of the current row.
 *
 * @param render The render data.
 */
static void	render_row(t_render *render)
{
	render->x = 0;
	while (render->x < render->width)
	{
		draw_pixel(render);
		render->x++;
	}
}

/**
 * @brief Draws the whole image, row by row.
 *
 * @param fractol The fractol state.
 */
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
