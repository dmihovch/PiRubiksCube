#include <cube.h>

/*
Cube rotation (tilt + joystick)
Call scroll_cube in here somewhere
For the direction:
1 = right
-1 = left
-2 = up
2 = down
These direction mappings may need to be changed after I figure out how to get the pi working again
*/
void remap_cube(Cube *cube, int direction) {
    Cube temp = *cube;
    if (direction == 1) { // right side becomes new top
        
    }
}


// Side rotations (action mode)

// Call scroll_row_left in here somewhere
void rotate_row_left(Cube *cube, int row) {

}

// Call scroll_row_right in here somewhere
void rotate_row_right(Cube *cube, int row) {

}

// Call scroll_col_up in here somewhere
void rotate_col_up(Cube *cube, int col) {

}

// Call scroll_col_down in here somewhere
void rotate_col_down(Cube *cube, int col) {

}