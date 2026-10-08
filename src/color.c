/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbraune <fbraune@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 20:07:06 by fbraune           #+#    #+#             */
/*   Updated: 2026/10/08 12:07:26 by fbraune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbraune <fbraune@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 20:08:34 by fbraune           #+#    #+#             */
/*   Updated: 2026/10/08 12:07:20 by fbraune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fractol.h"
#include <math.h>

/**
 * @brief Turns a number of steps into a colour.
 *
 * @param iter Number of steps the pixel needed.
 * @param cur_iter Steps the refinement is at right now.
 * @param max_iter Steps when the refinement is done.
 * @param shift How far the colours have moved.
 * @return The colour as one RGBA number.
 */
uint32_t	get_color(const int iter, const int cur_iter, const int max_iter,
		const double shift)
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
