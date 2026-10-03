#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>

typedef enum {
    TILT_NONE,
    TILT_LEFT,
    TILT_RIGHT,
    TILT_UP,
    TILT_DOWN,
} Tilt;

typedef enum {
    JOYSTICK_NONE,
    JOYSTICK_PRESS,
    JOYSTICK_LEFT,
    JOYSTICK_RIGHT,
    JOYSTICK_UP,
    JOYSTICK_DOWN,
} Joystick;

typedef struct {
    Tilt tilt;
    Joystick joystick;
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
