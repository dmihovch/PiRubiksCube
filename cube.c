#include <stdlib.h>
#include <time.h>
#include "cube.h"
#include "map.h"

void cube_init(Cube *cube) {
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

// Pick a random integer in [min, max]
static int rand_int(int min, int max) {
    return min + (rand() % (max - min + 1));
}

// Apply one random move from the move pool
static void apply_random_move(Cube *cube) {
    int move = rand_int(0, 15);  // 16 total moves

    if (move == 0) {
        remap_cube(cube, 1);
    } else if (move == 1) {
        remap_cube(cube, -1);
    } else if (move == 2) {
        remap_cube(cube, -2);
    } else if (move == 3) {
        remap_cube(cube, 2);
    }

    else if (move >= 4 && move <= 6) {
        int row = move - 4;  // 0,1,2
        rotate_row_left(cube, row);
    }
    else if (move >= 7 && move <= 9) {
        int row = move - 7;  // 0,1,2
        rotate_row_right(cube, row);
    }

    else if (move >= 10 && move <= 12) {
        int col = move - 10; // 0,1,2
        rotate_col_up(cube, col);
    }
    else if (move >= 13 && move <= 15) {
        int col = move - 13; // 0,1,2
        rotate_col_down(cube, col);
    }
}

// Scramble cube N times
void scramble_cube(Cube *cube, int times) {
    srand(time(NULL));  // seed RNG once per scramble call

    for (int i = 0; i < times; i++) {
        apply_random_move(cube);
    }
}