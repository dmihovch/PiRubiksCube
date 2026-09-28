#ifndef DISPLAY_H
#define DISPLAY_H

#include "cube.h"

#define WHITE 0xFFFF
#define BLACK 0x0000
#define RED 0xF800


// Handle if in SELECT or ACTION mode
typedef enum {
    MODE_SELECT,
    MODE_ACTION
} Mode;

// Opening and closing the display
bool open_display(void);
void close_display(void);
void clear_display(void);

// Draw the cube based on mode, selection, and tilt
void display_cube(const Cube *cube, Mode mode, int current_row, int current_col, int tilt);

void display_face_6x6(const int face[3][3]);
void display_preview(const Cube *cube, int tilt);

// Happens after an action
void scroll_cube(const Cube *cube, int direction);

void scroll_row_left(const Cube *cube, int row);
void scroll_row_right(const Cube *cube, int row);
void scroll_col_up(const Cube *cube, int col);
void scroll_col_down(const Cube *cube, int col);

#endif
