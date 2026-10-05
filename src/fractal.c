/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbraune <fbraune@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 20:05:43 by fbraune           #+#    #+#             */
/*   Updated: 2026/10/05 20:18:54 by fbraune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fractol.h"

/**
 * @brief Counts how long a point stays inside the Mandelbrot set.
 *
 * @param cr First part of the point.
 * @param ci Second part of the point.
 * @param max_iter Highest number of steps.
 * @return The number of steps that were done.
 */
int	mandelbrot_iter(double cr, double ci, int max_iter)
{
	double	zr;
	double	zi;
	double	tmp;
	int		i;

	zr = 0.0;
	zi = 0.0;
	i = 0;
	while (i < max_iter && (zr * zr + zi * zi) <= 4.0)
	{
		tmp = zr * zr - zi * zi + cr;
		zi = 2.0 * zr * zi + ci;
		zr = tmp;
		i++;
	}
	return (i);
}

/**
 * @brief Counts how long a point stays inside a Julia set.
 *
 * @param z The point. Gets the last value.
 * @param j The constant of the Julia set.
 * @param max_iter Highest number of steps.
 * @return The number of steps that were done.
 */
int	julia_iter(double z[2], double j[2], int max_iter)
{
	double	zr;
	double	zi;
	double	tmp;
	int		i;

	zr = z[0];
	zi = z[1];
	i = 0;
	while (i < max_iter && (zr * zr + zi * zi) <= 4.0)
	{
		tmp = zr * zr - zi * zi + j[0];
		zi = 2.0 * zr * zi + j[1];
		zr = tmp;
		i++;
	}
	z[0] = zr;
	z[1] = zi;
	return (i);
}

/**
 * @brief Does one step of the chaos game.
 *
 * @param i Number of the current step.
 * @param u Position on x, gets changed.
 * @param v Position on y, gets changed.
 * @param value Gets the number of the triangle that was hit.
 * @return true if the point fell into the hole.
 */
static bool	sierpinski_helper(int i, double *u, double *v, int *value)
{
	if (*u + *v <= 0.5)
	{
		*u *= 2.0;
		*v *= 2.0;
	}
	else if (*u >= 0.5)
	{
		*u = *u * 2.0 - 1.0;
		*v *= 2.0;
	}
	else if (*v >= 0.5)
	{
		*u *= 2.0;
		*v = *v * 2.0 - 1.0;
	}
	else
	{
		*value = (i);
		return (true);
	}
	return (false);
}

/**
 * @brief Plays the chaos game for one point of the triangle.
 *
 * @param x First part of the point.
 * @param y Second part of the point.
 * @param max_iter Highest number of steps.
 * @return The triangle number, or -1 if the point is outside.
 */
int	sierpinski_iter(double x, double y, int max_iter)
{
	int		i;
	double	u;
	double	v;
	int		value;

	u = (x + 1.0 - y) / 2.0;
	v = (1.0 - y - x) / 2.0;
	if (u < 0.0 || v < 0.0 || u + v > 1.0)
		return (-1);
	i = 0;
	while (i < max_iter)
	{
		if (sierpinski_helper(i, &u, &v, &value))
			return (value);
		i++;
	}
	return (i);
}
