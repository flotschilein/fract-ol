/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbraune <fbraune@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 19:57:59 by fbraune           #+#    #+#             */
/*   Updated: 2026/10/08 12:24:44 by fbraune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fractol.h"

#include <math.h>

/**
 * @brief Skips spaces and reads the + or - sign.
 *
 * @param val The string to read.
 * @param i Position in the string, moved forward.
 * @return 1 for a plus sign, -1 for a minus sign.
 */
static int	parse_sign(const char *val, int *i)
{
	int	sign;

	sign = 1;
	while (val[*i] == ' ' || (val[*i] >= '\t' && val[*i] <= '\r'))
		(*i)++;
	if (val[*i] == '-' || val[*i] == '+')
	{
		if (val[*i] == '-')
			sign = -1;
		(*i)++;
	}
	return (sign);
}

/**
 * @brief Reads the digits before the dot.
 *
 * @param val The string to read.
 * @param i Position in the string, moved forward.
 * @param has_digits Set to 1 if any digit was read.
 * @return The number that was read.
 */
static double	parse_integer(const char *val, int *i, int *has_digits)
{
	double	number;

	number = 0.0;
	while (val[*i] >= '0' && val[*i] <= '9')
	{
		number = number * 10.0 + (val[*i] - '0');
		*has_digits = 1;
		(*i)++;
	}
	return (number);
}

/**
 * @brief Reads the digits after the dot.
 *
 * @param val The string to read.
 * @param i Position in the string, moved forward.
 * @param divisor Gets a 10 for every digit after the dot.
 * @param has_digits Set to 1 if any digit was read.
 * @return The digits, not divided yet.
 */
static double	parse_fraction(const char *val, int *i, double *divisor,
		int *has_digits)
{
	double	fraction;

	fraction = 0.0;
	*divisor = 1.0;
	if (val[*i] == '.')
	{
		(*i)++;
		while (val[*i] >= '0' && val[*i] <= '9')
		{
			fraction = fraction * 10.0 + (val[*i] - '0');
			*divisor *= 10.0;
			*has_digits = 1;
			(*i)++;
		}
	}
	return (fraction);
}

/**
 * @brief Turns a string into a double. Stops the program if the string is
 *        not a number.
 *
 * @param value The string to read.
 * @return The number.
 */
double	parse_double(const char *value)
{
	double	num;
	double	div;
	int		sign;
	int		i;
	int		has_digits;

	if (!value || value[0] == '\0')
		write_error(NULL, USAGE);
	i = 0;
	has_digits = 0;
	sign = parse_sign(value, &i);
	num = parse_integer(value, &i, &has_digits);
	num += parse_fraction(value, &i, &div, &has_digits) / div;
	num *= sign;
	if (!has_digits || value[i] != '\0' || !isfinite(num))
		write_error(NULL, USAGE);
	return (num);
}
