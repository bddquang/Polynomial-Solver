# Polynomial Root Finder and ASCII Plotter

## Overview:
  This C program provides a menu-driven interface to find approximate roots of a polynomial using numerical methods (bisection and secant methods) and visualize the polynomial as an ASCII graph. It supports     polynomial input, root-finding, interval refinement, and graphical representation, with results logged to a file.

## Features:
- Polynomial Input: Enter polynomial degree (up to 10) and coefficients with input validation.
- Root Finding: Identify intervals containing roots with a specified step size, Narrow down root-containing intervals using the bisection method and approximate roots using the secant method with either fixed iterations or error tolerance ((|x_k - x_{k-1}|) or (|f(x_k)|/m)).
- ASCII Graph: Plot the polynomial as a text-based graph (80x24 canvas).
- Output Logging: Save all inputs, calculations, and graphs to ketqua.txt.
- Interactive Menu: Navigate via arrow keys and Enter, with highlighted selections and sub-menu for secant method options.

## Key Functions:
- nhap_da_thuc: Input polynomial degree and coefficients with error checking.
- gia_tri: Evaluate the polynomial at a given point using Horner's method.
- dao_ham: Compute the first derivative of the polynomial.
- nhap_dau_vao: Input interval ([a, b]) with validation a < b.
- tim_khoang_nghiem: Find intervals containing roots with a user-defined step.
- chia_doi: Refine root intervals using the bisection method (stop when interval width < 0.5).
- ve_do_thi: Plot the polynomial as an ASCII graph with axes and labeled coordinates.
- kiem_tra_day_cung: Validate conditions for the secant method.
- day_cung: Approximate roots using the secant method with a fixed number of iterations.
- day_cung_eps: Approximate roots using the secant method with error-based stopping conditions.
- menu & sub_menu: Interactive console menus with arrow key navigation and highlighted selections.

## Usage:
- Run the program and input the number of decimal places for output.
- Enter polynomial degree and coefficients.
- Navigate the menu using arrow keys and Enter to:
- Find root-containing intervals.
- Refine intervals or approximate roots.
- Plot the polynomial graph.
- Input a new polynomial or exit.
- Results are displayed on the console and saved to ketqua.txt.

## Requirements
C compiler (e.g., GCC)
Windows environment (uses windows.h for console handling with GetStdHandle and SetConsoleCursorPosition)

## Notes
- Run the program in Dev-C++ to ensure proper menu display, as it relies on Windows console functions (GetStdHandle and SetConsoleCursorPosition) for interactive navigation and highlighting.
- Maximum polynomial degree: 10 (defined by BAC_TOI_DA).
- Maximum iterations for secant method: 1000 (defined by LAN_LAP_TOI_DA).
- Output file: ketqua.txt (created in the working directory).

## Example
For polynomial ( f(x) = x^3 - x - 2 ):
- Input: Degree = 3, Coefficients = ([1, 0, -1, -2])
- Interval: ([1, 2])
- Output: Root-containing intervals, refined intervals, approximate root, and ASCII graph.
