/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbraune <fbraune@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/08 12:14:33 by fbraune           #+#    #+#             */
/*   Updated: 2026/10/08 12:24:41 by fbraune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fractol.h"

/**
 * @brief Waits a little, then adds a few iterations and draws again.
 *
 * @param fractol The fractol state.
 * @param dt Time since the last frame.
 */
void	iteration_helper(t_fractol *fractol, const double dt)
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

/**
 * @brief Zooms with the collected wheel movement once it stops.
 *
 * @param fractol The fractol state.
 * @param dt Time since the last frame.
 * @param moved Set to true if the view changed.
 */
void	scroll_helper(t_fractol *fractol, const double dt, bool *moved)
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
void	color_helper(t_fractol *fractol, const double dt, bool *redraw)
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
void	render_helper(t_fractol *fractol)
{
	fractol->iter = START_ITER;
	render_fractal(fractol);
}
