/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbraune <fbraune@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/08 12:11:50 by fbraune           #+#    #+#             */
/*   Updated: 2026/10/08 12:13:29 by fbraune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fractol.h"

/**
 * @brief Shifts the colours and draws again.
 *
 * @param keydata The key event. Holding SHIFT moves the colours back.
 * @param fractol The fractol state.
 */
void	color_shift(mlx_key_data_t keydata, t_fractol *fractol)
{
	if (keydata.modifier & MLX_SHIFT)
		fractol->color_shift -= 1.0;
	else
		fractol->color_shift += 1.0;
	render_fractal(fractol);
}

/**
 * @brief Gives the time since the last frame, kept small enough to be safe.
 *
 * @param fractol The fractol state.
 * @return The time in seconds.
 */
double	get_dt(const t_fractol *fractol)
{
	double	dt;

	dt = fractol->mlx->delta_time;
	if (dt < 0.0)
		dt = 0.0;
	if (dt > 0.05)
		dt = 0.05;
	return (dt);
}

/**
 * @brief Close callback. Closes the window.
 *
 * @param param The fractol state.
 */
void	close_hook(void *param)
{
	t_fractol	*fractol;

	fractol = (t_fractol *)param;
	mlx_close_window(fractol->mlx);
}

/**
 * @brief Cursor callback. Saves the mouse position.
 *
 * @param xpos Mouse position on x.
 * @param ypos Mouse position on y.
 * @param param The fractol state.
 */
void	cursor_hook(double xpos, double ypos, void *param)
{
	t_fractol	*fractol;

	fractol = (t_fractol *)param;
	fractol->mouse_x = (int)xpos;
	fractol->mouse_y = (int)ypos;
}
