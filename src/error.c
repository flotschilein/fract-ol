/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbraune <fbraune@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 19:57:55 by fbraune           #+#    #+#             */
/*   Updated: 2026/10/05 20:18:54 by fbraune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fractol.h"

/**
 * @brief Prints an error to the terminal and exits the program.
 *
 * @param fractol The fractol state, closes the window. Can be NULL.
 * @param msg The message to print. NULL prints "FATAL ERROR".
 */
void	write_error(t_fractol *fractol, char *msg)
{
	write(2, BOLD_RED, ft_strlen(BOLD_RED));
	if (!msg)
		write(2, "FATAL ERROR", ft_strlen("FATAL ERROR"));
	else
		write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	write(2, RESET, ft_strlen(RESET));
	if (fractol && fractol->mlx)
	{
		mlx_terminate(fractol->mlx);
		fractol->mlx = NULL;
		fractol->img = NULL;
	}
	exit(1);
}