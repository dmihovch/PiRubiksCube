#ifndef INPUT_GYRO_H
#define INPUT_GYRO_H

#include "sense.h"

// Input functions
void open_input(void);
void close_input(void);
void check_input(void (*callback)(unsigned int code), int delay);

// Interrupt handler
void interrupt_handler(int sig);

// Gyro functions
void open_gyro(void);
float check_gyroX(void);
float check_gyroY(void);
void close_gyro(void);

#endif