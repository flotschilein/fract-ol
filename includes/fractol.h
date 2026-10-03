/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbraune <fbraune@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/03 19:10:18 by fbraune           #+#    #+#             */
/*   Updated: 2026/10/03 19:45:32 by fbraune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

#include "colors.h"

#include "../Libft/libft.h"

typedef enum s_fractol_type {
    MANDELBROT,
    JULIA
} t_fratol_type;

typedef struct s_fractol {
    t_fratol_type type;
    double julia_x;
    double julia_y;
    int iterations;
    int max_iter;
    int win_width;
    int win_height;
    int offset_x;
    int offset_y;
} t_fractol;