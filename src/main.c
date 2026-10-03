/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbraune <fbraune@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/03 19:09:24 by fbraune           #+#    #+#             */
/*   Updated: 2026/10/03 21:07:11 by fbraune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fractol.h"

#include <errno.h>
#include <math.h>
#include <stdio.h>

void write_error(char *msg)
{
    write(1, BOLD_RED , ft_strlen(BOLD_RED));
    if (!msg)
        write(2, "FATAL ERROR", ft_strlen("FATAL ERROR"));
    else
        write(1, msg , ft_strlen(msg));
    write(2, "\n", 1);
    write(1, RESET , ft_strlen(RESET));
    exit(1);
}

double parse_double(const char *value)
{
    double  number;
    double  divisor;
    int     sign;
    int     i;
    int     has_digits;

    if (!value || value[0] == '\0') {
        write_error("Usage: ./fractol Mandelbrot | ./fractol Julia <real> <imaginary>");
    }
    number = 0.0;
    divisor = 1.0;
    sign = 1;
    i = 0;
    has_digits = 0;
    while (value[i] == ' ' || (value[i] >= '\t' && value[i] <= '\r'))
        i++;
    if (value[i] == '-' || value[i] == '+') {
        if (value[i] == '-')
            sign = -1;
        i++;
    }
    while (value[i] >= '0' && value[i] <= '9') {
        number = number * 10.0 + (value[i] - '0');
        has_digits = 1;
        i++;
    }
    if (value[i] == '.') {
        i++;
        while (value[i] >= '0' && value[i] <= '9') {
            number = number * 10.0 + (value[i] - '0');
            divisor *= 10.0;
            has_digits = 1;
            i++;
        }
        number /= divisor;
    }
    number *= sign;
    if (!has_digits || value[i] != '\0' || !isfinite(number)) {
        write_error("Usage: ./fractol Mandelbrot | ./fractol Julia <real> <imaginary>");
    }

    return (number);
}

void init_values(t_fractol *fractol)
{
    fractol->mlx = NULL;
    fractol->img = NULL;
    fractol->type = MANDELBROT;
    fractol->julia_x = 0.0;
    fractol->julia_y = 0.0;
    fractol->win_width = WIN_WIDTH;
    fractol->win_height = WIN_HEIGHT;
    fractol->zoom = 1.0;
    fractol->offset_x = 0;
    fractol->offset_y = 0;
    fractol->max_iter = MAX_ITER;
    fractol->iterations = MAX_ITER;
    fractol->color_shift = 0;
    fractol->mouse_x = 0;
    fractol->mouse_y = 0;
}

void debug_print(t_fractol *fractol)
{
    const char *type_str;

    if (fractol->type == MANDELBROT)
        type_str = "Mandelbrot";
    else
        type_str = "Julia";
    printf("--- fractol debug ---\n");
    printf("type        : %s (%d)\n", type_str, fractol->type);
    printf("julia_x     : %f\n", fractol->julia_x);
    printf("julia_y     : %f\n", fractol->julia_y);
    printf("zoom        : %f\n", fractol->zoom);
    printf("offset_x    : %d\n", fractol->offset_x);
    printf("offset_y    : %d\n", fractol->offset_y);
    printf("max_iter    : %d\n", fractol->max_iter);
    printf("iterations  : %d\n", fractol->iterations);
    printf("color_shift : %d\n", fractol->color_shift);
    printf("mouse       : (%d, %d)\n", fractol->mouse_x, fractol->mouse_y);
    printf("win         : %dx%d\n", fractol->win_width, fractol->win_height);
    printf("mlx         : %p\n", (void *)fractol->mlx);
    printf("img         : %p\n", (void *)fractol->img);
    printf("---------------------\n");
    fflush(stdout);
}

void key_hook(mlx_key_data_t keydata, void *param)
{
    t_fractol *fractol;

    fractol = (t_fractol *)param;
    if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
    {
        printf("[hook] ESC pressed, closing window\n");
        debug_print(fractol);
        mlx_close_window(fractol->mlx);
        return ;
    }
    if (keydata.action != MLX_PRESS && keydata.action != MLX_REPEAT)
        return ;
    if (keydata.key == MLX_KEY_UP || keydata.key == MLX_KEY_W)
        fractol->offset_y -= MOVE_STEP;
    else if (keydata.key == MLX_KEY_DOWN || keydata.key == MLX_KEY_S)
        fractol->offset_y += MOVE_STEP;
    else if (keydata.key == MLX_KEY_LEFT || keydata.key == MLX_KEY_A)
        fractol->offset_x -= MOVE_STEP;
    else if (keydata.key == MLX_KEY_RIGHT || keydata.key == MLX_KEY_D)
        fractol->offset_x += MOVE_STEP;
    else if (keydata.key == MLX_KEY_KP_ADD || keydata.key == MLX_KEY_EQUAL)
    {
        fractol->zoom *= ZOOM_FACTOR;
        if (fractol->zoom > ZOOM_MAX)
            fractol->zoom = ZOOM_MAX;
    }
    else if (keydata.key == MLX_KEY_KP_SUBTRACT || keydata.key == MLX_KEY_MINUS)
    {
        fractol->zoom /= ZOOM_FACTOR;
        if (fractol->zoom < ZOOM_MIN)
            fractol->zoom = ZOOM_MIN;
    }
    else
        return ;
    printf("[hook] key %d action %d -> offset (%d, %d) zoom %f\n",
        keydata.key, keydata.action,
        fractol->offset_x, fractol->offset_y, fractol->zoom);
    debug_print(fractol);
}

void close_hook(void *param)
{
    t_fractol *fractol;

    fractol = (t_fractol *)param;
    printf("[hook] window X button pressed, closing\n");
    debug_print(fractol);
    mlx_close_window(fractol->mlx);
}

void scroll_hook(double xdelta, double ydelta, void *param)
{
    t_fractol *fractol;

    (void)xdelta;
    fractol = (t_fractol *)param;
    if (ydelta == 0)
        return ;
    mlx_get_mouse_pos(fractol->mlx, &fractol->mouse_x, &fractol->mouse_y);
    if (ydelta > 0)
    {
        fractol->zoom *= ZOOM_FACTOR;
        if (fractol->zoom > ZOOM_MAX)
            fractol->zoom = ZOOM_MAX;
    }
    else
    {
        fractol->zoom /= ZOOM_FACTOR;
        if (fractol->zoom < ZOOM_MIN)
            fractol->zoom = ZOOM_MIN;
    }
    printf("[hook] scroll y %f -> zoom %f at mouse (%d, %d)\n",
        ydelta, fractol->zoom, fractol->mouse_x, fractol->mouse_y);
    debug_print(fractol);
}

void cursor_hook(double xpos, double ypos, void *param)
{
    t_fractol *fractol;

    fractol = (t_fractol *)param;
    fractol->mouse_x = (int)xpos;
    fractol->mouse_y = (int)ypos;
}

int main(int argc, const char **argv)
{
    t_fractol  fractol;

    init_values(&fractol);
    if (argc == 2 && ft_strcmp((char *)argv[1], "Mandelbrot") == 0)
        fractol.type = MANDELBROT;
    else if (argc == 4 && ft_strcmp((char *)argv[1], "Julia") == 0)
    {
        fractol.type = JULIA;
        fractol.julia_x = parse_double(argv[2]);
        fractol.julia_y = parse_double(argv[3]);
    }
    else
        write_error("Usage: ./fractol Mandelbrot | ./fractol Julia <real> <imaginary>");
    fractol.mlx = mlx_init(fractol.win_width, fractol.win_height,
            "fract-ol", false);
    if (!fractol.mlx)
        write_error("mlx_init failed");
    debug_print(&fractol);
    mlx_key_hook(fractol.mlx, key_hook, &fractol);
    mlx_close_hook(fractol.mlx, close_hook, &fractol);
    mlx_scroll_hook(fractol.mlx, scroll_hook, &fractol);
    mlx_cursor_hook(fractol.mlx, cursor_hook, &fractol);
    mlx_loop(fractol.mlx);
    mlx_terminate(fractol.mlx);
    return (0);
}
