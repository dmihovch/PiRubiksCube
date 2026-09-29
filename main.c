#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "sense.h"

#include "display.h"
#include "input.h"
#include "cube.h"
#include "map.h"

static void process_select_mode(Cube *cube, InputState *in, int *current_row, int *current_col, Mode *mode);
static void process_action_mode(Cube *cube, InputState *in, int current_row, int current_col, Mode *mode);

// Able to read from the command line / makes terminal input non‑blocking.
static void enable_nonblocking_stdin(void) {
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
}

// Reads whatever you typed without pausing the program and exit if we type exit
static int check_exit_command(void) {
    char buf[32];
    ssize_t n = read(STDIN_FILENO, buf, sizeof(buf)-1);
    if (n > 0) {
        buf[n] = '\0';
        if (strstr(buf, "exit") || strstr(buf, "quit") || strstr(buf, "q")) {
            return 1;
        }
    }
    return 0;
}

int main(int argc, char **argv) {
    // Type exit, quit or q to quit the program
    enable_nonblocking_stdin();

    Cube cube;
    Mode mode = MODE_SELECT;

    int current_row = 0;   // selected row (0–2)
    int current_col = 0;   // selected col (0–2)

    // Initialize cube to solved state
    cube_init(&cube);

    // Handle command-line scramble
    if (argc >= 2 && strcmp(argv[1], "scramble") == 0) {
        int times = (argc == 3) ? atoi(argv[2]) : 100; // Default scramble is 100
        scramble_cube(&cube, times);
    }

if (!open_input() || !open_gyro() || !open_display()) {
    close_all_devices();
    return EXIT_FAILURE;
}

    // Main loop
    while (true) {
        InputState in = {0};
        read_input(&in);

        printf("Gyro raw code = %d | Row=%d | Col=%d\n", in.tilt, current_row, current_col);

        if (mode == MODE_SELECT) {
            process_select_mode(&cube, &in, &current_row, &current_col, &mode);
        } else {
            process_action_mode(&cube, &in, current_row, current_col, &mode);
        }

        // Draw cube based on current mode + tilt
        display_cube(&cube, mode, current_row, current_col, in.tilt);

        if (check_exit_command()) {
            printf("Exiting...\n");
            break;
        }

        usleep(40); // ~25 FPS
    }

    close_all_devices();
    return EXIT_SUCCESS;
}

static void process_select_mode(Cube *cube, InputState *in, int *current_row, int *current_col, Mode *mode) {

    // Move selection cursor
    if (in->joystick == 2) (*current_col) = (*current_col + 2) % 3; // left
    if (in->joystick == -2)  (*current_col) = (*current_col + 1) % 3; // right
    if (in->joystick == -1) (*current_row) = (*current_row + 2) % 3; // up
    if (in->joystick == 1)  (*current_row) = (*current_row + 1) % 3; // down

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
    if (in->joystick == -2) rotate_row_right(cube, current_row);
    if (in->joystick == 2) rotate_row_left(cube, current_row);

    // Column rotations
    if (in->joystick == 1) rotate_col_up(cube, current_col);
    if (in->joystick == -1) rotate_col_down(cube, current_col);

    // Click returns to SELECT mode
    if (in->joystick == 99) {
        *mode = MODE_SELECT;
    }
}


