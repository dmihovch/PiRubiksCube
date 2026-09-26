#ifndef MAP_H
#define MAP_H

#include "cube.h"

// Cube rotation (tilt + joystick)
void remap_cube(Cube *cube, int direction);

// Side rotations (action mode)
void rotate_row_left(Cube *cube, int row);
void rotate_row_right(Cube *cube, int row);
void rotate_col_up(Cube *cube, int col);
void rotate_col_down(Cube *cube, int col);

#endif