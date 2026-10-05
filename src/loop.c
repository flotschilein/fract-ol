/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbraune <fbraune@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 20:08:21 by fbraune           #+#    #+#             */
/*   Updated: 2026/10/05 20:18:54 by fbraune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fractol.h"

#include <math.h>

/**
 * @brief Waits a little, then adds a few iterations and draws again.
 *
 * @param fractol The fractol state.
 * @param dt Time since the last frame.
 */
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

/**
 * @brief Resizes the image when the window size changed.
 *
 * @param fractol The fractol state.
 * @param moved Set to true if the image changed.
 */
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

/**
 * @brief Zooms with the collected wheel movement once it stops.
 *
 * @param fractol The fractol state.
 * @param dt Time since the last frame.
 * @param moved Set to true if the view changed.
 */
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

/**
 * @brief Moves the colours along for the colour animation.
 *
 * @param fractol The fractol state.
 * @param dt Time since the last frame.
 * @param redraw Set to true if a new drawing is needed.
 */
static void	color_helper(t_fractol *fractol, double dt, bool *redraw)
{
	fractol->color_shift += dt * AUTO_COLOR_SPEED;
	fractol->color_shift = fmod(fractol->color_shift, COLOR_SHIFT_WRAP);
	*redraw = true;
}

/**
 * @brief Sets the iterations back to the start value and draws again.
 *
 * @param fractol The fractol state.
 */
static void	render_helper(t_fractol *fractol)
{
	fractol->iter = START_ITER;
	render_fractal(fractol);
}

/**
 * @brief Loop callback. Runs once per frame.
 *
 * @param param The fractol state.
 */
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
