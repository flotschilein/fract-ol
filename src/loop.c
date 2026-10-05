/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbraune <fbraune@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 20:08:21 by fbraune           #+#    #+#             */
/*   Updated: 2026/10/05 20:08:23 by fbraune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fractol.h"

#include <math.h>

static void	iteration_helper(t_fractol *fractol, double dt)
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

static void	resize_helper(t_fractol *fractol, bool *moved)
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

static void	scroll_helper(t_fractol *fractol, double dt, bool *moved)
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

static void	color_helper(t_fractol *fractol, double dt, bool *redraw)
{
	fractol->color_shift += dt * AUTO_COLOR_SPEED;
	fractol->color_shift = fmod(fractol->color_shift, COLOR_SHIFT_WRAP);
	*redraw = true;
}

static void	render_helper(t_fractol *fractol)
{
	fractol->iter = START_ITER;
	render_fractal(fractol);
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
