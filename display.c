#include <unistd.h> // sleep()
#include <cube.h>
#include <display.h>
#include "sense.h"

// Opening and closing the display

pi_framebuffer_t *fb = NULL;
void clear_display(void) {
    clearFrameBuffer(fb,BLACK);
}

void open_display(void) {
    if (fb && fb != NULL) {
        clear_display();
    }
    fb=getFrameBuffer();
    sense_fb_bitmap_t *bm=fb->bitmap;
}

void close_display(void) {
        if (fb != NULL) {
                clearFrameBuffer(fb,BLACK);
                freeFrameBuffer(fb);
                fb = 0;
        }
}

// main controller for how the cube should be displayed at a certain point in time
// called by the main loop
void display_cube(const Cube *cube, Mode mode, int current_row, int current_col, int tilt) {

}

// pi laying flat / "default" view
void display_face_6x6(const int face[3][3]) {

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
