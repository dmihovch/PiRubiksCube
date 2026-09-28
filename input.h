#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>

typedef struct {
    int tilt;        // -1 = left, 1 = right, -2 = up, 2 = down, 0 = none
    int joystick;    // same mapping as tilt
} InputState;

// Master close function
void close_all_devices(void);

// Read all input
void read_input(InputState *state);

// Interrupt handler
void interrupt_handler(int sig);

// Input functions
bool open_input(void);
void check_joystick(unsigned int code);
void close_input(void);

// Gyro functions
bool open_gyro(void);
void close_gyro(void);

#endif