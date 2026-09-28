#include <string.h>

#include "map.h"
#include "cube.h"

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

// Helper functions

// Clockwise rottion of a face
void rotate_face_cw(int face[3][3]) {
    int temp[3][3];
    memcpy(temp, face, sizeof(temp));

    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            face[c][2 - r] = temp[r][c];
        }
    }
}

// Counter clockwise rotation of a face
void rotate_face_ccw(int face[3][3]) {
    int temp[3][3];
    memcpy(temp, face, sizeof(temp));

    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            face[2 - c][r] = temp[r][c];
        }
    }
}

// PRIVATE helper — only used by remap_cube
static void invert_face_map(int dest[3][3], int src[3][3]) {
    // invert 3x3 (flip both axes)
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            dest[2 - r][2 - c] = src[r][c];
        }
    }
}

// PRIVATE helper — only used by row/column rotations
static void invert_row_map(int dest[3], int src[3]) {
    // reverse the row: [0,1,2] → [2,1,0]
    dest[0] = src[2];
    dest[1] = src[1];
    dest[2] = src[0];
}


// Main Control Functions

void remap_cube(Cube *cube, int direction) {
    Cube temp = *cube;

    if (direction == 1) { // right side becomes new top. Means tilt was left (-1)
        memcpy(cube->top, temp.right, sizeof(cube->top)); // right --> top
        memcpy(cube->left, temp.top, sizeof(cube->top)); // top --> left

        // left --> bottom
        // left[0][0] --> bottom[2][2]
        // bottom --> right
        // bottom[0][0] --> right[2][2]
        invert_face_map(cube->bottom, temp.left);
        invert_face_map(cube->right, temp.bottom);

        // up 90 degrees clockwise
        // up[0][0] --> up[0][2]
        // dwon 90 degreese counter-clockwise
        // up[0][0] --> up[2][0]
        rotate_face_cw(cube->up);
        rotate_face_ccw(cube->down);

    } else if (direction == -1) {
        memcpy(cube->top, temp.left, sizeof(cube->top)); // left --> top
        memcpy(cube->right, temp.top, sizeof(cube->top)); // top --> right
        
        invert_face_map(cube->bottom, temp.right);
        invert_face_map(cube->left, temp.bottom);

        rotate_face_cw(cube->down);
        rotate_face_ccw(cube->up);

    } else if (direction == -2) {
        memcpy(cube->top, temp.up, sizeof(cube->top));
        memcpy(cube->down, temp.top, sizeof(cube->top));
        memcpy(cube->bottom, temp.down, sizeof(cube->top));
        memcpy(cube->up, temp.bottom, sizeof(cube->top));

        rotate_face_cw(cube->left);
        rotate_face_ccw(cube->right);

    } else if (direction == 2) {
        memcpy(cube->top, temp.down, sizeof(cube->top));
        memcpy(cube->up, temp.top, sizeof(cube->top));
        memcpy(cube->bottom, temp.up, sizeof(cube->top));
        memcpy(cube->down, temp.bottom, sizeof(cube->top));

        rotate_face_cw(cube->right);
        rotate_face_ccw(cube->left);

    }
}


// Side rotations (action mode)

// Call scroll_row_left in here somewhere
void rotate_row_left(Cube *cube, int row) {
    Cube temp = *cube;

    memcpy(cube->top[row], temp.right[row], sizeof(cube->top[3]));
    memcpy(cube->left[row], temp.top[row], sizeof(cube->top[3]));

    invert_row_map(cube->bottom[2 - row], temp.left[row]);
    invert_row_map(cube->right[row], temp.bottom[2 - row]);

    if (row == 0) {
        rotate_face_cw(cube->up);
    } else if (row == 2) {
        rotate_face_ccw(cube->down);
    }
}

// Call scroll_row_right in here somewhere
void rotate_row_right(Cube *cube, int row) {
    Cube temp = *cube;

    memcpy(cube->top[row], temp.left[row], sizeof(cube->top[3]));
    memcpy(cube->right[row], temp.top[row], sizeof(cube->top[3]));

    invert_row_map(cube->bottom[2 - row], temp.right[row]);
    invert_row_map(cube->left[row], temp.bottom[2 - row]);

    if (row == 0) {
        rotate_face_ccw(cube->up);
    } else if (row == 2) {
        rotate_face_cw(cube->down);
    }
}

// Call scroll_col_up in here somewhere
void rotate_col_up(Cube *cube, int col) {
    Cube temp = *cube;

    for (int i = 0; i < 3; i++) {
        cube->top[i][col] = temp.down[i][col];
        cube->down[i][col] = temp.bottom[i][col];
        cube->bottom[i][col] = temp.up[i][col];
        cube->up[i][col] = temp.top[i][col];
    }

    if (col == 0) {
        rotate_face_ccw(cube->left);
    } else if (col == 2) {
        rotate_face_cw(cube->right);
    }
}

// Call scroll_col_down in here somewhere
void rotate_col_down(Cube *cube, int col) {
    Cube temp = *cube;

    for (int i = 0; i < 3; i++) {
        cube->top[i][col] = temp.up[i][col];
        cube->up[i][col] = temp.bottom[i][col];
        cube->bottom[i][col] = temp.down[i][col];
        cube->down[i][col] = temp.top[i][col];
    }

    if (col == 0) {
        rotate_face_cw(cube->left);
    } else if (col == 2) {
        rotate_face_ccw(cube->right);
    }
}