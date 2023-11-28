#include "send_to_arduino.h"

#define ARDUINO_SERIAL_DEVICE "/dev/ttyUSB0"

int open_comm_arduino() {
	int arduino_fd;
	if ((arduino_fd = serialOpen(ARDUINO_SERIAL_DEVICE, 9600)) < 0 ) {
		fprintf(stderr, "Unable to open serial device: %s\n", ARDUINO_SERIAL_DEVICE);
		return 1;
	}
	
	return arduino_fd;
}

//Call seperateley for each integer you want to send
//code 1: send next position
void send_code_to_arduino(int port, int code) {
	char received_data[64];
	char received_char;
	int ack = 0;
	time_t start, current;
	int timeout = 5;
	
	//send code
	char codeStr[4];
	snprintf(codeStr, sizeof(codeStr), "%03d", code);
	serialPrintf(port, codeStr);
	
	//wait for acknowldegment
	time(&start);
	while(!ack && difftime(time(&current), start) < timeout) {
		while (serialDataAvail(port) > 0) {
			serialGetchar(port);
			if (received_char != -1) {
				strncat(received_data, &received_char, 1);
			}
		}
		if (strstr(received_data, "ACK") == NULL) {
				ack = 1;
		}
	}
}

void send_next_point_to_arduino(int port, Point * next, Point * current) {
	send_code_to_arduino(port, 1);
	
	float x0 = (float)current->x;
	float y0 = (float)current->y;
	float x1 = (float)next->x;
	float y1 = (float)next->y;
	
	serialPrintf(port, "%.2f\n%.2f\n%.2f\n%.2f\n", x0, y0, x1, y1);	
}


void close_comm_arduino(int port) {
	serialClose(port);
}
