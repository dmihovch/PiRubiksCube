#include <cube.h>

static void cube_init(Cube *cube) {
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            cube->top[row][col] = 0;
            cube->bottom[row][col] = 1;
            cube->left[row][col] = 2;
            cube->right[row][col] = 3;
            cube->up[row][col] = 4;
            cube->down[row][col] = 5;
        }
    }
}

// Scramble cube N times
void scramble_cube(Cube *cube, int times) {
    
}