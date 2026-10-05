/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbraune <fbraune@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/03 19:09:24 by fbraune           #+#    #+#             */
/*   Updated: 2026/10/05 20:05:43 by fbraune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fractol.h"

int	main(int argc, const char **argv)
{
	t_fractol	fractol;

	set_values(argc, argv, &fractol);
	render_fractal(&fractol);
	mlx_resize_hook(fractol.mlx, resize_hook, &fractol);
	mlx_key_hook(fractol.mlx, key_hook, &fractol);
	mlx_close_hook(fractol.mlx, close_hook, &fractol);
	mlx_scroll_hook(fractol.mlx, scroll_hook, &fractol);
	mlx_cursor_hook(fractol.mlx, cursor_hook, &fractol);
	mlx_loop_hook(fractol.mlx, loop_hook, &fractol);
	mlx_loop(fractol.mlx);
	mlx_terminate(fractol.mlx);
	return (0);
}
