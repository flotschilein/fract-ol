/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbraune <fbraune@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 20:08:21 by fbraune           #+#    #+#             */
/*   Updated: 2026/10/08 12:24:43 by fbraune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fractol.h"

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
