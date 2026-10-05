/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbraune <fbraune@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 20:08:34 by fbraune           #+#    #+#             */
/*   Updated: 2026/10/05 20:08:36 by fbraune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fractol.h"

#include <math.h>

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
