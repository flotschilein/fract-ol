/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbraune <fbraune@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 20:05:43 by fbraune           #+#    #+#             */
/*   Updated: 2026/10/05 20:05:43 by fbraune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fractol.h"

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
