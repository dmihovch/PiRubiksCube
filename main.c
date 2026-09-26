#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

// Not sure if this is the right libsense and currently have no idea 
// how to use it since I dont have the pi infront of me
#include "sense.h"

#include "display.h"
#include "map.h"
#include "cube.h"

// NEED TO FIGURE OUT HOW TO USE JOYSTICK AGAIN
// -------------------------------
// Gyro + Joystick State
// -------------------------------
typedef struct {
    int tilt;        // -1 = left, 1 = right, -2 = up, 2 = down, 0 = none
    int joystick;    // same mapping as tilt
} InputState;

    // Prototypes
    static void read_input(InputState *state);
    static void process_select_mode(Cube *cube, InputState *in, int *current_row, int *current_col, Mode *mode);
    static void process_action_mode(Cube *cube, InputState *in, int current_row, int current_col, Mode *mode);

int main(int argc, char **argv) {
    Cube cube;
    Mode mode = MODE_SELECT;

    int current_row = 0;   // selected row (0–2)
    int current_col = 0;   // selected col (0–2)

    // Initialize libsense hardware
    // This is chated because I dont have a PI infront of me right now 
    // and completely forget how to do stuff on the raspberry pi
    if (!sense_init()) { // <-- probably needs to be changed
        fprintf(stderr, "Failed to initialize Sense HAT\n");
        return EXIT_FAILURE;
    }

    // Initialize cube to solved state
    cube_init(&cube);

    // Handle command-line scramble
    if (argc >= 2 && strcmp(argv[1], "scramble") == 0) {
        int times = (argc == 3) ? atoi(argv[2]) : 100; // Default scramble is 100
        scramble_cube(&cube, times);
    }

    // Main loop
    while (true) {
        InputState in = {0};
        read_input(&in);

        if (mode == MODE_SELECT) {
            process_select_mode(&cube, &in, &current_row, &current_col, &mode);
        } else {
            process_action_mode(&cube, &in, current_row, current_col, &mode);
        }

        // Draw cube based on current mode + tilt
        display_cube(&cube, mode, current_row, current_col, in.tilt);

        sense_sleep_ms(40); // ~25 FPS
    }

    sense_close();
    return EXIT_SUCCESS;
}


// This function definetly has to be reworked for libsense
// Or i just need to figure out how it works again
static void read_input(InputState *state) {
    sense_event_t e;

    // Gyro
    float roll = sense_get_roll();
    float pitch = sense_get_pitch();

    if (roll > 20) state->tilt = 1;       // right
    else if (roll < -20) state->tilt = -1; // left
    else if (pitch > 20) state->tilt = -2; // up
    else if (pitch < -20) state->tilt = 2; // down
    else state->tilt = 0;

    // Joystick
    if (sense_poll_event(&e)) {
        if (e.type == SENSE_JOYSTICK) {
            switch (e.code) {
                case SENSE_JOY_UP:    state->joystick = -2; break;
                case SENSE_JOY_DOWN:  state->joystick = 2; break;
                case SENSE_JOY_LEFT:  state->joystick = -1; break;
                case SENSE_JOY_RIGHT: state->joystick = 1; break;
                case SENSE_JOY_CLICK: state->joystick = 99; break;
            }
        }
    }
}

static void process_select_mode(Cube *cube, InputState *in, int *current_row, int *current_col, Mode *mode) {

    // Move selection cursor
    if (in->joystick == -1) (*current_col) = (*current_col + 2) % 3; // left
    if (in->joystick == 1)  (*current_col) = (*current_col + 1) % 3; // right
    if (in->joystick == -2) (*current_row) = (*current_row + 2) % 3; // up
    if (in->joystick == 2)  (*current_row) = (*current_row + 1) % 3; // down

    // Tilt + joystick = cube rotation
    if (in->tilt != 0 && in->joystick == in->tilt) {
        remap_cube(cube, in->tilt);
    }

    // Click = switch to ACTION mode
    if (in->joystick == 99) {
        *mode = MODE_ACTION;
    }
}

static void process_action_mode(Cube *cube, InputState *in, int current_row, int current_col, Mode *mode) {

    // Row rotations
    if (in->joystick == 1) rotate_row_right(cube, current_row);
    if (in->joystick == -1) rotate_row_left(cube, current_row);

    // Column rotations
    if (in->joystick == -2) rotate_col_up(cube, current_col);
    if (in->joystick == 2) rotate_col_down(cube, current_col);

    // Click returns to SELECT mode
    if (in->joystick == 99) {
        *mode = MODE_SELECT;
    }
}


