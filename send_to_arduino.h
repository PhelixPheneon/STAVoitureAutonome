#ifndef SENDTOARDUINO_H_
#define SENDTOARDUINO_H_

#include "main.h"

int open_comm_arduino();

void send_to_arduino(int port, int code);

void send_next_point_to_arduino(int port, Point next, Point current);

void close_comm_arduino(int port);

#endif
