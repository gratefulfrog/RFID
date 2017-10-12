/*
 * common_uart.h
 *
 *  Created on: 2013-5-29
 *      Author: pasino
 */
 
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>


#ifndef COMMON_UART_H_
#define COMMON_UART_H_


#define UINT8 		unsigned char


//鎵撳紑涓插彛
//@param dev 璁惧鍦板潃
//@param speed 閫熺巼
//@param databits 鏁版嵁浣�
//@param stopbis 鍋滄浣�
//@param parity 鏍￠獙浣� 'n','N' 鏃犳牎楠屼綅锛�'o','O' 濂囨牎楠岋紝'e','E' 鍋舵牎楠� 锛�'s','S'鏃犳牎楠�
//@param readTime 璇诲彇瓒呮椂鏃堕棿
extern int uart_open(char* dev, int speed, int databits, int stopbis,int parity, int readTime);

//璁剧疆閫熺巼
//@param fd 涓插彛鎵撳紑鐨勫湴鍧�
//@param speed 涓插彛閫熺巼
extern int set_uart_speed(int fd, int speed);

//璁剧疆鏁版嵁浣嶏紝鍋滄浣嶏紝鏍￠獙浣�
//@param fd 涓插彛鎵撳紑浣嶇疆
//@param databits 鏁版嵁浣�
//@param stopbis 鍋滄浣�
//@param parity 鏍￠獙浣� 'n','N' 鏃犳牎楠屼綅锛�'o','O' 濂囨牎楠岋紝'e','E' 鍋舵牎楠� 锛�'s','S'鏃犳牎楠�
//@param readTime 璇诲彇瓒呮椂鏃堕棿
extern int set_uart_parity(int fd, int databits, int stopbits, int parity, int readTime);

#endif /* COMMON_UART_H_ */
