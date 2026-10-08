/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbraune <fbraune@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 20:08:09 by fbraune           #+#    #+#             */
/*   Updated: 2026/10/08 12:24:47 by fbraune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fractol.h"

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
