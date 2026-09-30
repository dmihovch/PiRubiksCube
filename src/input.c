#include <linux/input.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "sense.h"
#include "../include/display.h"
#include "../include/input.h"

pi_joystick_t *joystick = NULL;
pi_i2c_t* device = NULL;

static InputState *polling_state = NULL;

int run=1;
void interrupt_handler(int sig){
    run=0;
}

void close_all_devices(void) {
    close_input();
    close_gyro();
    close_display();
}

void read_input(InputState *state) { // <-- state is already a pointer. We pass in the address of in (&in)
    coordinate_t orientation = {0.0, 0.0, 0.0};

    polling_state = state; // <-- This is a pointer to a pointer to an address. So now polling_state = &in
    pollJoystick(joystick, check_joystick, 40);
    polling_state = NULL; // <-- Doing this doesnt change in since it is just a pointer. It's value it holds is an address

    if (getGyroPosition(device, &orientation)) {
        double pitch = orientation.x;
        double roll  = orientation.y;

        if (pitch > 20)       state->tilt = TILT_UP;
        else if (pitch < -20) state->tilt = TILT_DOWN;
        else if (roll > 20)   state->tilt = TILT_LEFT;
        else if (roll < -20)  state->tilt = TILT_RIGHT;
    }
}

bool open_input(void) {
    joystick = getJoystickDevice();
    // Could not open joystick --> Error
    if (joystick == NULL) {
        fprintf(stderr, "Could not find the Sense HAT joystick\n");
        close_all_devices();
        return false;
    }
    return true;
}

void check_joystick(unsigned int code) {
    if (polling_state == NULL) return;

    // Right here is when the pointer polling_state is dereferenced, turning it basically into in (like in.joystick)
    switch (code) {
        case KEY_UP:    polling_state->joystick = JOYSTICK_UP; break;
        case KEY_DOWN:  polling_state->joystick = JOYSTICK_DOWN; break;
        case KEY_LEFT:  polling_state->joystick = JOYSTICK_LEFT; break;
        case KEY_RIGHT: polling_state->joystick = JOYSTICK_RIGHT; break;
        case KEY_ENTER: polling_state->joystick = JOYSTICK_PRESS; break;
    }
}


void close_input(void) {
    if (joystick != NULL) {
        freeJoystick(joystick);
        joystick = NULL;
    }
}

bool open_gyro() {
    coordinate_t data = {0,0,0};
    signal(SIGINT, interrupt_handler);

    device = geti2cDevice();
    // Could not open device --> Error
    if (device == NULL) {
        fprintf(stderr, "Could not open a Sense HAT I2C device\n");
        close_all_devices();
        return false;
    }

    configureAccelGyro(device);
    // Could not configure gyro --> Error
    if (!configureAccelGyro(device)) {
        fprintf(stderr, "Could not configure the Sense HAT accelerometer/gyro\n");
        close_all_devices();
        return false;
    }

    printf("Please leave the Pi flat on the table for calibration\n");
    sleep(1);
    while(run) {
        if (!getGyroPosition(device, &data)) break;
        if (data.x != 0.0) break;
        usleep(100);
    }
    printf("You may pick up the pi.\nStarting in ...\n");
    sleep(1);
    printf("3\n");
    sleep(1);
    printf("2\n");
    sleep(1);
    printf("1\n");
    sleep(1);

    return true;
}

void close_gyro() {
    if (device != NULL) {
        freei2cDevice(device);
        device = NULL;
    }
}
