#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <sys/time.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include "jetsonGPIO.h"

using namespace std;

int getkey() {
    int character;
    struct termios orig_term_attr;
    struct termios new_term_attr;

    /* set the terminal to raw mode */
    tcgetattr(fileno(stdin), &orig_term_attr);
    memcpy(&new_term_attr, &orig_term_attr, sizeof(struct termios));
    new_term_attr.c_lflag &= ~(ECHO|ICANON);
    new_term_attr.c_cc[VTIME] = 0;
    new_term_attr.c_cc[VMIN] = 0;
    tcsetattr(fileno(stdin), TCSANOW, &new_term_attr);

    /* read a character from the stdin stream without blocking */
    /*   returns EOF (-1) if no character is available */
    character = fgetc(stdin);

    /* restore the original terminal attributes */
    tcsetattr(fileno(stdin), TCSANOW, &orig_term_attr);

    return character;
}


int main(int argc, char* argv[]) {
	cout << "Testing the GPIO Pins" << endl;

	jetsonTX2GPIONumber tiltSensor = gpio388;
	unsigned int value = low;
	
	while(getkey() != 27) {
		gpioGetValue(tiltSensor, &value) ;

		if (value==high) {
		    cout << "Tilt Sensor HIGH" << endl;
		} else {
		    cout << "Tilt Sensor LOW" << endl;
		}
		usleep(1000); // sleep for a millisecond
    	}

	gpioUnexport(tiltSensor);      // unexport the tilt sensor

	return 0;
}
