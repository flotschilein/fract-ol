/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbraune <fbraune@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/03 19:09:24 by fbraune           #+#    #+#             */
/*   Updated: 2026/10/03 21:30:36 by fbraune          ###   ########.fr       */
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
    fractol->offset_x = 0.0;
    fractol->offset_y = 0.0;
    fractol->max_iter = MAX_ITER;
    fractol->iterations = MAX_ITER;
    fractol->color_shift = 0;
    fractol->auto_color = false;
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
    printf("offset_x    : %f\n", fractol->offset_x);
    printf("offset_y    : %f\n", fractol->offset_y);
    printf("max_iter    : %d\n", fractol->max_iter);
    printf("iterations  : %d\n", fractol->iterations);
    printf("color_shift : %d\n", fractol->color_shift);
    printf("auto_color  : %d\n", fractol->auto_color);
    printf("mouse       : (%d, %d)\n", fractol->mouse_x, fractol->mouse_y);
    printf("win         : %dx%d\n", fractol->win_width, fractol->win_height);
    printf("mlx         : %p\n", (void *)fractol->mlx);
    printf("img         : %p\n", (void *)fractol->img);
    printf("---------------------\n");
    fflush(stdout);
}

void apply_zoom(t_fractol *fractol, double factor, double anchor_x, double anchor_y)
{
    double  new_zoom;
    double  eff;
    double  cx;
    double  cy;

    new_zoom = fractol->zoom * factor;
    if (new_zoom < ZOOM_MIN)
        new_zoom = ZOOM_MIN;
    if (new_zoom > ZOOM_MAX)
        new_zoom = ZOOM_MAX;
    eff = new_zoom / fractol->zoom;
    cx = fractol->win_width / 2.0;
    cy = fractol->win_height / 2.0;
    fractol->offset_x = fractol->offset_x * eff + (anchor_x - cx) * (1.0 - eff);
    fractol->offset_y = fractol->offset_y * eff + (anchor_y - cy) * (1.0 - eff);
    fractol->zoom = new_zoom;
}

void key_hook(mlx_key_data_t keydata, void *param)
{
    t_fractol *fractol;
    bool      moved;

    fractol = (t_fractol *)param;
    moved = false;
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
    {
        fractol->offset_y -= MOVE_STEP;
        moved = true;
    }
    else if (keydata.key == MLX_KEY_DOWN || keydata.key == MLX_KEY_S)
    {
        fractol->offset_y += MOVE_STEP;
        moved = true;
    }
    else if (keydata.key == MLX_KEY_LEFT || keydata.key == MLX_KEY_A)
    {
        fractol->offset_x -= MOVE_STEP;
        moved = true;
    }
    else if (keydata.key == MLX_KEY_RIGHT || keydata.key == MLX_KEY_D)
    {
        fractol->offset_x += MOVE_STEP;
        moved = true;
    }
    else if (keydata.key == MLX_KEY_KP_ADD || keydata.key == MLX_KEY_EQUAL)
    {
        apply_zoom(fractol, ZOOM_FACTOR,
            fractol->win_width / 2.0, fractol->win_height / 2.0);
    }
    else if (keydata.key == MLX_KEY_KP_SUBTRACT || keydata.key == MLX_KEY_MINUS)
    {
        apply_zoom(fractol, 1.0 / ZOOM_FACTOR,
            fractol->win_width / 2.0, fractol->win_height / 2.0);
    }
    else if (keydata.key == MLX_KEY_C)
    {
        if (keydata.modifier & MLX_SHIFT)
            fractol->color_shift -= 1;
        else
            fractol->color_shift += 1;
    }
    else if (keydata.key == MLX_KEY_V)
    {
        if (keydata.action != MLX_PRESS)
            return ;
        fractol->auto_color = !fractol->auto_color;
    }
    else
        return ;
    if (moved)
        render_fractal(fractol);
    printf("[hook] key %d action %d -> offset (%f, %f) zoom %f color %d auto %d\n",
        keydata.key, keydata.action,
        fractol->offset_x, fractol->offset_y, fractol->zoom,
        fractol->color_shift, fractol->auto_color);
    debug_print(fractol);
}

void loop_hook(void *param)
{
    t_fractol       *fractol;
    static int      frame;

    fractol = (t_fractol *)param;
    if (!fractol->auto_color)
    {
        frame = 0;
        return ;
    }
    frame++;
    if (frame >= AUTO_COLOR_EVERY_N_FRAMES)
    {
        frame = 0;
        fractol->color_shift += 1;
    }
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
    double      factor;

    (void)xdelta;
    fractol = (t_fractol *)param;
    if (ydelta == 0)
        return ;
    mlx_get_mouse_pos(fractol->mlx, &fractol->mouse_x, &fractol->mouse_y);
    factor = pow(ZOOM_FACTOR, ydelta);
    apply_zoom(fractol, factor,
        (double)fractol->mouse_x, (double)fractol->mouse_y);
    printf("[hook] scroll y %f -> zoom %f offset (%f, %f) at mouse (%d, %d)\n",
        ydelta, fractol->zoom, fractol->offset_x, fractol->offset_y,
        fractol->mouse_x, fractol->mouse_y);
    debug_print(fractol);
}

void cursor_hook(double xpos, double ypos, void *param)
{
    t_fractol *fractol;

    fractol = (t_fractol *)param;
    fractol->mouse_x = (int)xpos;
    fractol->mouse_y = (int)ypos;
}

void resize_hook(int32_t width, int32_t height, void *param)
{
    t_fractol *fractol;

    fractol = (t_fractol *)param;
    if (width <= 0 || height <= 0)
        return ;
    fractol->win_width = width;
    fractol->win_height = height;
    if (fractol->img)
    {
        if (!mlx_resize_image(fractol->img, (uint32_t)width, (uint32_t)height))
        {
            write_error("mlx_resize_image failed");
        }
        render_fractal(fractol);
    }
    printf("[hook] resize -> %dx%d\n", width, height);
}

int mandelbrot_iter(double cr, double ci, int max_iter)
{
    double  zr;
    double  zi;
    double  tmp;
    int     i;

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

uint32_t get_color_simple(int iter, int max_iter)
{
    uint8_t r;
    uint8_t g;
    uint8_t b;

    if (iter >= max_iter)
        return (0x000000FF);
    r = (uint8_t)((iter * 9) % 255);
    g = (uint8_t)((iter * 7) % 255);
    b = (uint8_t)((iter * 13) % 255);
    return ((uint32_t)(r << 24 | g << 16 | b << 8 | 0xFF));
}

void render_fractal(t_fractol *fractol)
{
    int     x;
    int     y;
    double  cr;
    double  ci;
    int     iter;

    if (!fractol || !fractol->img)
        return ;
    if (fractol->type != MANDELBROT)
        return ;
    y = 0;
    while (y < fractol->win_height)
    {
        x = 0;
        while (x < fractol->win_width)
        {
            cr = ((double)x - fractol->win_width / 2.0 + fractol->offset_x)
                * (4.0 / fractol->win_width);
            ci = ((double)y - fractol->win_height / 2.0 + fractol->offset_y)
                * (4.0 / fractol->win_height);
            iter = mandelbrot_iter(cr, ci, fractol->max_iter);
            mlx_put_pixel(fractol->img, (uint32_t)x, (uint32_t)y,
                get_color_simple(iter, fractol->max_iter));
            x++;
        }
        y++;
    }
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
            "fract-ol", true);
    if (!fractol.mlx)
        write_error("mlx_init failed");
    fractol.img = mlx_new_image(fractol.mlx,
            (uint32_t)fractol.win_width, (uint32_t)fractol.win_height);
    if (!fractol.img)
        write_error("mlx_new_image failed");
    if (mlx_image_to_window(fractol.mlx, fractol.img, 0, 0) < 0)
        write_error("mlx_image_to_window failed");
    render_fractal(&fractol);
    debug_print(&fractol);
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
