#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "include/cube.h"
#include "include/display.h"
#include "include/input.h"
#include "include/map.h"

static void process_select_mode(Cube *cube, InputState *in, int *current_row, int *current_col, Mode *mode);
static void process_action_mode(Cube *cube, InputState *in, int current_row, int current_col, Mode *mode);

// True when the joystick is pushed in the same direction as the tilt
static bool tilt_joystick_same(Tilt tilt, Joystick joystick) {
    switch (tilt) {
        case TILT_LEFT:  return joystick == JOYSTICK_LEFT;
        case TILT_RIGHT: return joystick == JOYSTICK_RIGHT;
        case TILT_UP:    return joystick == JOYSTICK_UP;
        case TILT_DOWN:  return joystick == JOYSTICK_DOWN;
        default:         return false;
    }
}

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

    int current_row = 1;   // selected row (0–2), start at center
    int current_col = 1;   // selected col (0–2), start at center

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

    long long unsigned int cnt = 0;
    // Main loop
    while (true) {
        InputState in = {0};
        read_input(&in);


        if (cnt % 10000) {
            printf("Joystick = %d | Tilt = %d | Row=%d | Col=%d\n", (int)in.joystick, (int)in.tilt, current_row, current_col);
        }
        cnt++;


        if(mode == MODE_SELECT){
            process_select_mode(&cube, &in, &current_row, &current_col, &mode);
        } else {
            process_action_mode(&cube, &in, current_row, current_col, &mode);
        }

        // Draw cube based on current mode + tilt
        clear_display();
        display_cube(&cube, mode, current_row, current_col, in.tilt);

        if (check_exit_command()) {
            printf("Exiting...\n");
            break;
        }

        usleep(4000); // ~25 FPS
    }

    close_all_devices();
    return EXIT_SUCCESS;
}

Tilt opposite_tilt(Tilt tilt) {
    switch (tilt) {
        case TILT_LEFT:  return TILT_RIGHT;
        case TILT_RIGHT: return TILT_LEFT;
        case TILT_UP:    return TILT_DOWN;
        case TILT_DOWN:  return TILT_UP;
        default:         return tilt;
    }
}

static void process_select_mode(Cube *cube, InputState *in, int *current_row, int *current_col, Mode *mode) {

    // Move selection cursor
    if(!in->tilt)
    {
        if (in->joystick == JOYSTICK_LEFT)  (*current_col) = (*current_col + 2) % 3;
        if (in->joystick == JOYSTICK_RIGHT) (*current_col) = (*current_col + 1) % 3;
        if (in->joystick == JOYSTICK_UP)    (*current_row) = (*current_row + 2) % 3;
        if (in->joystick == JOYSTICK_DOWN)  (*current_row) = (*current_row + 1) % 3;
    }

    // Tilt + joystick = cube rotation
    if (tilt_joystick_same(in->tilt, in->joystick)) {
        remap_cube(cube, opposite_tilt(in->tilt));
    }

    // Click = switch to ACTION mode
    if (in->joystick == JOYSTICK_PRESS) {
        *mode = MODE_ACTION;
    }
}

static void process_action_mode(Cube *cube, InputState *in, int current_row, int current_col, Mode *mode) {

    // Row rotations
    if (in->joystick == JOYSTICK_RIGHT) rotate_row_left(cube, current_row);
    if (in->joystick == JOYSTICK_LEFT)  rotate_row_right(cube, current_row);
                                    //   ^ to match \/ that
    // Column rotations
    if (in->joystick == JOYSTICK_DOWN) rotate_col_up(cube, current_col);
    if (in->joystick == JOYSTICK_UP)   rotate_col_down(cube, current_col);

    // Click returns to SELECT mode
    if (in->joystick == JOYSTICK_PRESS) {
        *mode = MODE_SELECT;
    }
}
