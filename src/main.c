/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbraune <fbraune@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/03 19:09:24 by fbraune           #+#    #+#             */
/*   Updated: 2026/10/03 19:33:15 by fbraune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fractol.h"

void write_error(char *msg) {
    write(1, BOLD_RED , ft_strlen(BOLD_RED));
    if (!msg)
        write(2, "FATAL ERROR", ft_strlen("FATAL ERROR"));
    else
        write(1, msg , ft_strlen(msg));
    write(1, RESET , ft_strlen(RESET));
}

int main(int argc, const char **argv) {
    if (argc < 2) {
        write_error("Usage: ./fractol Julia, Mandelbrot");
    }
    if (argc == 2) {
        if (ft_strcmp(argv[1], "Mandelbrot") != 0) {
            write_error("Usage: ./fractol Julia, Mandelbrot");
        }
    }
    if (argc == 4) {
        if (ft_strcmp(argv[1], "Julia") != 0) {
            write_error("Usage: ./fractol Julia, Mandelbrot");
        }

    }
    else
        write_error("Usage: ./fractol Julia, Mandelbrot");
}