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

// Helper functions

void rotate_face_cw(int face[3][3]) {
    int temp[3][3];
    memcpy(temp, face, sizeof(temp));

    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            face[c][2 - r] = temp[r][c];
        }
    }
}

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

// Main Control Functions

void remap_cube(Cube *cube, int direction) {
    Cube temp = *cube;

    if (direction == 1) { // right side becomes new top. Means tilt was left (-1)
        memcpy(cube->top, temp.right, sizeof(cube->top)); // right --> top
        memcpy(cube->left, temp.top, sizeof(cube->top)); // top --> left

        // left --> bottom
        // left[0][0] --> bottom[2][2]
        //
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