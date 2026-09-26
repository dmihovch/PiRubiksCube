#ifndef CUBE_H
#define CUBE_H

#include <stdint.h>
#include <map.h>

typedef struct {
    int top[3][3];
    int bottom[3][3];
    int left[3][3];
    int right[3][3];
    int up[3][3];
    int down[3][3];
} Cube;

void cube_init(Cube *cube);

// Scramble cube N times
void scramble_cube(Cube *cube, int times);

#endif
