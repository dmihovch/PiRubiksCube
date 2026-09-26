#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <linux/input.h>

//#include "sense.h" // Need to fix

#include "display.h"
#include "map.h"
#include "cube.h"

typedef struct {
    int tilt;        // -1 = left, 1 = right, -2 = up, 2 = down, 0 = none
    int joystick;    // same mapping as tilt
} InputState;

static InputState *polling_state;

static void joystick_event(unsigned int code);
static void read_input(InputState *state, pi_joystick_t *joystick, pi_i2c_t *imu);
static void process_select_mode(Cube *cube, InputState *in, int *current_row, int *current_col, Mode *mode);
static void process_action_mode(Cube *cube, InputState *in, int current_row, int current_col, Mode *mode);

int main(int argc, char **argv) {
    Cube cube;
    Mode mode = MODE_SELECT;

    int current_row = 0;   // selected row (0–2)
    int current_col = 0;   // selected col (0–2)

    pi_i2c_t *imu = geti2cDevice();
    if (imu == NULL) {
        fprintf(stderr, "Could not open a Sense HAT I2C device\n");
        return EXIT_FAILURE;
    }
    pi_joystick_t *joystick = getJoystickDevice();
    if (joystick == NULL) {
        fprintf(stderr, "Could not find the Sense HAT joystick\n");
        freei2cDevice(imu);
        return EXIT_FAILURE;
    }
    if (!configureAccelGyro(imu)) {
        fprintf(stderr, "Could not configure the Sense HAT accelerometer/gyro\n");
        freeJoystick(joystick);
        freei2cDevice(imu);
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
        read_input(&in, joystick, imu);

        if (mode == MODE_SELECT) {
            process_select_mode(&cube, &in, &current_row, &current_col, &mode);
        } else {
            process_action_mode(&cube, &in, current_row, current_col, &mode);
        }

        // Draw cube based on current mode + tilt
        display_cube(&cube, mode, current_row, current_col, in.tilt);

        //sense_sleep_ms(40); // ~25 FPS
    }

    freeJoystick(joystick);
    freei2cDevice(imu);
    return EXIT_SUCCESS;
}

static void read_input(InputState *state, pi_joystick_t *joystick, pi_i2c_t *imu) {
    coordinate_t orientation = {0.0, 0.0, 0.0};

    polling_state = state;
    pollJoystick(joystick, joystick_event, 40);
    polling_state = NULL;

    if (getGyroPosition(imu, &orientation)) {
        double pitch = orientation.x;
        double roll = orientation.y;

        if (roll > 20) state->tilt = 1;
        else if (roll < -20) state->tilt = -1;
        else if (pitch > 20) state->tilt = -2;
        else if (pitch < -20) state->tilt = 2;
    }
}

static void joystick_event(unsigned int code) {
    if (polling_state == NULL) return;

    switch (code) {
        case KEY_UP:    polling_state->joystick = -2; break;
        case KEY_DOWN:  polling_state->joystick = 2; break;
        case KEY_LEFT:  polling_state->joystick = -1; break;
        case KEY_RIGHT: polling_state->joystick = 1; break;
        case KEY_ENTER: polling_state->joystick = 99; break;
    }
}

static void process_select_mode(Cube *cube, InputState *in, int *current_row, int *current_col, Mode *mode) {

    // Move selection cursor
    if (in->joystick == -1) (*current_col) = (*current_col + 2) % 3; // left
    if (in->joystick == 1)  (*current_col) = (*current_col + 1) % 3; // right
    if (in->joystick == -2) (*current_row) = (*current_row + 2) % 3; // up
    if (in->joystick == 2)  (*current_row) = (*current_row + 1) % 3; // down

    // Tilt + joystick = cube rotation
    if (in->tilt != 0 && in->joystick == (-1 * in->tilt)) {
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


