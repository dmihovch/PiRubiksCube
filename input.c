#include <unistd.h> // Used for sleep()
#include "sense.h" // used for all sense hat stuff
#include <stdlib.h> // used for free()
//#include <linux/input.h> // May not be used since we are using the sense librairy (according to copilot)
#include <stdio.h> // for printf()
#include <signal.h> // used for signal()
#define _GNU_SOURCE

#define WHITE 0xFFFF
#define BLACK 0x0000

pi_joystick_t *joystick = NULL;
pi_i2c_t* device = NULL;
coordinate_t data = {0,0,0};

void open_input(void) {
    if (joystick == NULL) {
        joystick = getJoystickDevice();
    }
}

void close_input(void) {
    if (joystick != NULL) {
	free(joystick);
	joystick = NULL; 
    }
}

void check_input(void (*callback)(unsigned int code), int delay) {
        pollJoystick(joystick, callback, delay);
}

int run=1;
void interrupt_handler(int sig){
    run=0;
}

void open_gyro() {
    if (device == NULL) {
	signal(SIGINT, interrupt_handler);
    	device=geti2cDevice();
    	if (device){
        	configureAccelGyro(device);
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
	}
	}
}

float check_gyroX() {
        if (run && getGyroPosition(device,&data)){
                return data.x;
        }
}

float check_gyroY() {
        if (run && getGyroPosition(device,&data)){
                return data.y;
        }
}

void close_gyro() {
        freei2cDevice(device);
	device = NULL;
}
