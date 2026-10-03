/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbraune <fbraune@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/03 19:09:24 by fbraune           #+#    #+#             */
/*   Updated: 2026/10/03 19:38:53 by fbraune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fractol.h"

void write_error(char *msg) {
    write(1, BOLD_RED , ft_strlen(BOLD_RED));
    if (!msg)
        write(2, "FATAL ERROR", ft_strlen("FATAL ERROR"));
    else
        write(1, msg , ft_strlen(msg));
    write(2, "\n", 1);
    write(1, RESET , ft_strlen(RESET));
    exit(1);
}

int main(int argc, const char **argv) {
    t_fractol fractol;
    if (argc < 2) {
        write_error("Usage: ./fractol Julia, Mandelbrot");
    }
    if (argc == 2) {
        if (ft_strcmp(argv[1], "Mandelbrot") != 0) {
            write_error("Usage: ./fractol Julia, Mandelbrot");
        }
        fractol.type = MANDELBROT;
    }
    if (argc == 4) {
        if (ft_strcmp(argv[1], "Julia") != 0) {
            write_error("Usage: ./fractol Julia, Mandelbrot");
        }
        fractol.type = JULIA;
    }
    else {
        write_error("Usage: ./fractol Julia, Mandelbrot");
    }
    return (0);
}