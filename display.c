#include <unistd.h> // sleep()
#include <stdbool.h>
#include <stdio.h>

#include "sense.h"
#include "cube.h"
#include "display.h"

#define WHITE 0xFFFF
#define BLACK 0x0000
#define RED 0xF800
#define YELLOW 0xFFE0
#define GREEN 0x07E0
#define BLUE 0x001F
#define ORANGE 0xFC00

static uint16_t color_to_rgb(int color) {
    switch (color) {
        case 0: return WHITE;
        case 1: return YELLOW;
        case 2: return RED;
        case 3: return ORANGE;
        case 4: return GREEN;
        case 5: return BLUE;
        default: return BLACK;
    }
}

// Opening and closing the display

pi_framebuffer_t *fb = NULL;

void clear_display(void) {
    if (fb != NULL) {
        clearFrameBuffer(fb, BLACK);
    }
}

bool open_display(void) {
    if (fb != NULL) {
        // Already open — clear it instead of reopening
        clearFrameBuffer(fb, BLACK);
        return true;
    }

    fb = getFrameBuffer();
    if (fb == NULL) {
        fprintf(stderr, "ERROR: Could not open Sense HAT framebuffer\n");
        return false;
    }

    clearFrameBuffer(fb, BLACK);
    return true;
}

void close_display(void) {
    if (fb != NULL) {
        clearFrameBuffer(fb,BLACK);
        freeFrameBuffer(fb);
        fb = NULL;
    }
}

// main controller for how the cube should be displayed at a certain point in time
// called by the main loop
void display_cube(const Cube *cube, Mode mode, int current_row, int current_col, int tilt) {
    display_face_6x6(cube->top);
}

// pi laying flat / "default" view
void display_face_6x6(const int face[3][3]) {
    if (fb == NULL) return;

    sense_fb_bitmap_t *bm = fb->bitmap;

    bm->pixel[0][0] = WHITE;
    bm->pixel[0][7] = RED;
    bm->pixel[7][0] = GREEN;
    bm->pixel[7][7] = BLUE;
    
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {

            uint16_t color = color_to_rgb(face[r][c]);

            int led_row = r * 2;
            int led_col = c * 2;

            // Fill 2×2 block
            bm->pixel[led_row][led_col] = color;
            bm->pixel[led_row][led_col + 1] = color;
            bm->pixel[led_row + 1][led_col] = color;
            bm->pixel[led_row + 1][led_col + 1] = color;
        }
    }
}

// Tilt pi in direction to preview that side
// rotate to side when joystick and tilt values are opposite
// (see NOTES in README)
void display_preview(const Cube *cube, int tilt) {

}

// Happens after an action
void scroll_row_left(const Cube *cube, int row) {

}

void scroll_row_right(const Cube *cube, int row) {

}

void scroll_col_up(const Cube *cube, int col) {

}

void scroll_col_down(const Cube *cube, int col) {

}
