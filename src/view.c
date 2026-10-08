/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   view.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbraune <fbraune@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 20:08:00 by fbraune           #+#    #+#             */
/*   Updated: 2026/10/08 12:24:57 by fbraune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fractol.h"

/**
 * @brief Zooms the view and keeps one point in the same place.
 *
 * @param fractol The fractol state.
 * @param factor Zoom factor, above 1 zooms in.
 * @param anchor_x The point that stays still, on x.
 * @param anchor_y The point that stays still, on y.
 */
void	apply_zoom(t_fractol *fractol, const double factor,
		const double anchor_x, double anchor_y)
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

/**
 * @brief Gives the middle point of the fractal that was chosen.
 *
 * @param fractol The fractol state.
 * @param center_x Gets the middle on x.
 * @param center_y Gets the middle on y.
 */
void	set_center(const t_fractol *fractol, double *center_x, double *center_y)
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
