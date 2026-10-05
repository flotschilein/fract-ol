/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbraune <fbraune@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 19:58:10 by fbraune           #+#    #+#             */
/*   Updated: 2026/10/05 20:18:54 by fbraune          ###   ########.fr       */
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
 * @brief Moves the view while a movement key is held down.
 *
 * @param fractol The fractol state.
 * @param step How far to move, in pixels.
 * @param moved Set to true if the view was moved.
 */
void	move_helper(t_fractol *fractol, double step, bool *moved)
{
	if (mlx_is_key_down(fractol->mlx, MLX_KEY_UP)
		|| mlx_is_key_down(fractol->mlx, MLX_KEY_W))
	{
		fractol->offset_y += step;
		*moved = true;
	}
	if (mlx_is_key_down(fractol->mlx, MLX_KEY_DOWN)
		|| mlx_is_key_down(fractol->mlx, MLX_KEY_S))
	{
		fractol->offset_y -= step;
		*moved = true;
	}
	if (mlx_is_key_down(fractol->mlx, MLX_KEY_LEFT)
		|| mlx_is_key_down(fractol->mlx, MLX_KEY_A))
	{
		fractol->offset_x -= step;
		*moved = true;
	}
	if (mlx_is_key_down(fractol->mlx, MLX_KEY_RIGHT)
		|| mlx_is_key_down(fractol->mlx, MLX_KEY_D))
	{
		fractol->offset_x += step;
		*moved = true;
	}
}

/**
 * @brief Gives the time since the last frame, kept small enough to be safe.
 *
 * @param fractol The fractol state.
 * @return The time in seconds.
 */
double	get_dt(t_fractol *fractol)
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
 * @brief Zooms in or out when a zoom key is pressed.
 *
 * @param keydata The key event.
 * @param fractol The fractol state.
 */
void	handle_zoom(mlx_key_data_t keydata, t_fractol *fractol)
{
	if (keydata.key == MLX_KEY_KP_ADD || keydata.key == MLX_KEY_EQUAL)
	{
		apply_zoom(fractol, ZOOM_FACTOR, fractol->win_width / 2.0,
			fractol->win_height / 2.0);
		fractol->iter = START_ITER;
		render_fractal(fractol);
	}
	else if (keydata.key == MLX_KEY_KP_SUBTRACT || keydata.key == MLX_KEY_MINUS)
	{
		apply_zoom(fractol, 1.0 / ZOOM_FACTOR, fractol->win_width / 2.0,
			fractol->win_height / 2.0);
		fractol->iter = START_ITER;
		render_fractal(fractol);
	}
}

/**
 * @brief Key callback. Handles closing, zooming and the colours.
 *
 * @param keydata The key event.
 * @param param The fractol state.
 */
void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_fractol	*fractol;

	fractol = (t_fractol *)param;
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
	{
		mlx_close_window(fractol->mlx);
		return ;
	}
	if (keydata.action != MLX_PRESS && keydata.action != MLX_REPEAT)
		return ;
	handle_zoom(keydata, fractol);
	if (keydata.key == MLX_KEY_C)
		color_shift(keydata, fractol);
	else if (keydata.key == MLX_KEY_V && keydata.action == MLX_PRESS)
		fractol->auto_color = !fractol->auto_color;
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
 * @brief Scroll callback. Keeps the wheel movement for the zoom.
 *
 * @param xdelta Side ways wheel movement, not used.
 * @param ydelta Wheel movement up and down.
 * @param param The fractol state.
 */
void	scroll_hook(double xdelta, double ydelta, void *param)
{
	t_fractol	*fractol;

	(void)xdelta;
	fractol = (t_fractol *)param;
	if (ydelta == 0)
		return ;
	mlx_get_mouse_pos(fractol->mlx, &fractol->mouse_x, &fractol->mouse_y);
	fractol->scroll_accum += ydelta;
	fractol->scroll_anchor_x = (double)fractol->mouse_x;
	fractol->scroll_anchor_y = (double)(fractol->win_height - fractol->mouse_y);
	fractol->scroll_timer = 0.0;
	fractol->scroll_pending = true;
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

/**
 * @brief Resize callback. Saves the new window size. The image itself is
 *        resized later in the loop.
 *
 * @param width New width.
 * @param height New height.
 * @param param The fractol state.
 */
void	resize_hook(int32_t width, int32_t height, void *param)
{
	t_fractol	*fractol;

	fractol = (t_fractol *)param;
	if (width <= 0 || height <= 0)
		return ;
	if (width == fractol->win_width && height == fractol->win_height
		&& !fractol->needs_resize)
		return ;
	fractol->win_width = width;
	fractol->win_height = height;
	fractol->needs_resize = true;
}
