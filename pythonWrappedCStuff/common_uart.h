/*
 * common_uart.h
 *
 *  Created on: 2013-5-29
 *      Author: pasino
 */
#ifndef COMMON_UART_H_
#define COMMON_UART_H_
 
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

extern int set_uart_parity(int fd, int databits, int stopbits, int parity, int readTime);
extern int set_uart_speed(int fd, int speed);
extern int uart_open(char* dev, int speed, int databits, int stopbis,int parity,int readTime);

#endif /* COMMON_UART_H_ */
