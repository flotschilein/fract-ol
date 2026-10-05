*This project has been created as part of the 42 curriculum by fbraune.*

# fract-ol

## Description

`fract-ol` is an interactive fractal viewer written in C. It renders three classic
fractals — the **Mandelbrot set**, the **Julia set** and the **Sierpinski
triangle** — inside a real-time graphical window that can be panned, zoomed and
re-coloured while it is running.

The goal of the project is to build a small, readable and fully hand-written
computer-graphics application that puts together the whole 42 workflow: a
library of my own (`Libft`), a strict C codebase, an external graphics library
(MLX42/GLFW/OpenGL), a custom build system and a modular source layout.

**How it works.** Every fractal is expressed as an *escape-time* algorithm: for
each pixel the program iterates a simple mathematical rule and counts how many
steps are needed before the point escapes (Mandelbrot/Julia) or lands in the
middle of a removed triangle (Sierpinski). That count is then mapped to a
colour. Rendering a full 800x800 window at 2000 iterations is far too slow for a
smooth framerate, so the viewer starts at 100 iterations and *refines*
progressively — every few milliseconds it adds a few iterations and redraws.
The result is a fast first image that becomes sharper while you look at it.

**Technical choices**

- **Language / build:** C11 with `gcc`, compiled with `-Wall -Wextra -Werror`
  and optimised with `-Ofast`. A hand-written Makefile builds every
  translation unit; no CMake, no autotools.
- **Graphics:** [MLX42](https://github.com/codam-coding-college/MLX42), a
  42-school image library on top of GLFW and OpenGL, used for the window,
  the frame buffer and all input events.
- **Library:** `Libft`, my own version of a subset of libc, used for string
  handling.
- **Module layout:** one translation unit per concern (`render`, `fractal`,
  `color`, `view`, `loop`, `input`, `init`, `parse`, `error`) with all
  prototypes in a single header, `includes/fractol.h`.
- **State:** the entire viewer state lives in one `t_fractol` struct that is
  passed to every callback, so the code has no globals.

## Instructions

### 1. Prerequisites

| Requirement | Notes |
|---|---|
| `gcc` | any recent version |
| `make` | GNU make |
| `cmake` | only to build MLX42 |
| `git` | used to clone MLX42 automatically |
| GLFW + OpenGL headers | e.g. `libglfw3-dev` and `libgl1-mesa-dev` |

### 2. Compilation

```bash
make
```

`make` compiles `Libft`, clones and builds MLX42 on first run, then links the
`fractol` binary in the repository root.

Other useful targets:

| Command | Effect |
|---|---|
| `make` | build everything |
| `make run` | build and run with the Mandelbrot set |
| `make run_julia` | run the Julia set (`JULIA_X` / `JULIA_Y` are defined in the Makefile) |
| `make run_sierpinski` | build and run the Sierpinski triangle |
| `make clean` | remove object files and the MLX42 build |
| `make fclean` | also remove `Libft/libft.a` and the binary |
| `make re` | full rebuild from scratch |

### 3. Execution

```bash
./fractol Mandelbrot
./fractol Sierpinski
./fractol Julia <real> <imaginary>
```

The Julia constant is given as two `double`s, for example:

```bash
./fractol Julia -0.4 0.6
./fractol Julia 0.285 0.01
./fractol Julia -0.7269 0.1889
```

Anything else prints the usage message and exits with status `1`.

### 4. Controls

| Input | Action |
|---|---|
| `←` `↑` `→` `↓` or `W` `A` `S` `D` | pan the fractal |
| `+` / `KP_ADD` | zoom in |
| `-` / `KP_SUBTRACT` | zoom out |
| mouse wheel | zoom towards the cursor |
| `C` | shift the colour palette (`Shift` + `C` shifts backwards) |
| `V` | toggle the animated colour cycle |
| `Esc` | close the window |

Resizing the window re-renders the fractal at the new size.

## Features

- three fractals selectable from the command line
- continuous, cursor-anchored zooming
- keyboard and mouse navigation, live window resizing
- animated colour cycling and manual palette shifting
- progressive refinement for a responsive framerate
- strict `-Werror` build with a modular, fully separated source layout

## Project structure

```
fract-ol/
├── Makefile
├── README.md
├── includes/
│   ├── colors.h        # terminal colour codes for error output
│   └── fractol.h       # shared types, defines and all prototypes
├── src/
│   ├── main.c          # main(): wires everything together
│   ├── init.c          # default values, argument parsing, window creation
│   ├── error.c         # write_error(): error output + cleanup
│   ├── parse.c         # parse_double(): strict double parsing
│   ├── input.c         # every MLX callback (key, mouse, cursor, resize)
│   ├── loop.c          # per-frame update logic
│   ├── view.c          # zoom and centre calculations
│   ├── render.c        # pixel/row drawing loop
│   ├── fractal.c       # Mandelbrot, Julia and Sierpinski iterators
│   └── color.c         # iteration count -> RGB colour
├── Libft/              # personal C library
└── mlx42/              # cloned automatically by make (not in git)
```

## Resources

### Documentation and references

- **Mandelbrot set** — the original definition and history:
  [Wolfram MathWorld](https://mathworld.wolfram.com/MandelbrotSet.html),
  and the free book *Gems of Chaos* by Jan Gulden:
  <https://mathroughthegems.wordpress.com/>
- **Julia sets** — [MathWorld](https://mathworld.wolfram.com/JuliaSet.html)
- **Sierpinski triangle** — [MathWorld](https://mathworld.wolfram.com/SierpinskiTriangle.html)
- **Escape-time colouring / smooth iteration counts** —
  [Wikipedia: Escape-time algorithm](https://en.wikipedia.org/wiki/Escape-time_algorithm)
- **MLX42 documentation** (window creation, image buffers, hooks):
  <https://42-codam-coding-college.github.io/MLX42/>
- **GLFW** (window and input backend used by MLX42):
  <https://www.glfw.org/documentation.shtml>
- **OpenGL** — <https://www.opengl.org/documentation/>
- **42 subject: C Piscine / Norm** — the coding standards (`norm`) and
  `man`/`man_campus` pages used throughout the project.

### Use of AI

AI (an LLM coding assistant) was used during the development of this project
for the following tasks:

- **Code splitting and modularisation.** The original single-file
  `src/main.c` was divided into the modules listed above. The assistant chose
  the module boundaries, moved each function to the file where it logically
  belongs, turned file-local helpers into `static` functions, and generated
  the matching prototypes in `includes/fractol.h`.
- **Docstrings.** The Doxygen-style docstrings (`@brief`, `@param`,
  `@return`) in front of the functions in `src/*.c` were written by the AI
  assistant from the existing code. Every one of them was reviewed and
  corrected by hand, and the file headers, structure of the project and all
  code comments that are not docstrings are handwritten.
- **Documentation.** This `README.md` was drafted with AI assistance; the
  project description, structure and controls were derived from the code, and
  the "Use of AI" section above was written by hand.

No AI-generated code was accepted without being compiled and tested locally,
and all algorithmic parts (the three escape-time loops, the colour mapping and
the progressive refinement) were written by hand.
