#include <unistd.h> // Used for sleep()
#include "sense.h" // used for all sense hat stuff
#include <stdlib.h> // used for free()
//#include <linux/input.h> // May not be used since we are using the sense librairy (according to copilot)
#include <stdio.h> // for printf()
#include <signal.h> // used for signal()
#define _GNU_SOURCE

#include "input.h"
#include "display.h"

pi_joystick_t *joystick = NULL;
pi_i2c_t* device = NULL;
coordinate_t data = {0,0,0};

int run=1;
void interrupt_handler(int sig){
    run=0;
}

void close_all_devices(void) {
    close_input();
    close_gyro();
    close_display();
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

void close_input(void) {
    if (joystick != NULL) {
        freeJoystick(joystick);
        joystick = NULL;
    }
}

void check_input(void (*callback)(unsigned int code), int delay) {
    pollJoystick(joystick, callback, delay);
}

bool open_gyro() {
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
    while(run && getGyroPosition(device,&data) && data.x==0.0) {
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

float check_gyroX() {
    if (run && getGyroPosition(device,&data)){
            return data.x;
    }
    return 0.0f;
}

float check_gyroY() {
    if (run && getGyroPosition(device,&data)){
            return data.y;
    }
    return 0.0f;
}

void close_gyro() {
    if (device != NULL) {
        freei2cDevice(device);
        device = NULL;
    }
}

