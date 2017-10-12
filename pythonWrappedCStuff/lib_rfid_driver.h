#ifndef LIB_RFID_DRIVER_H_
#define LIB_RFID_DRIVER_H_

#include "common_uart.h"
#include <termios.h>
#include <string.h>
#include <stdio.h>

#define PORT     "/dev/ttyUSB0"

int bytes2HexString(char *src, char *des, int size);

int receive_serial_bytes(UINT8 *pszBuffer, int iLength, int uart_fd);

int send_serial_bytes(UINT8 *pszData, int iLength, int uart_fd);

unsigned char RecvUhfUartData(unsigned char *rbuf,unsigned char *rlen);

unsigned char SendCommand_R2000(unsigned char* SendCommand, unsigned short SendNum);

void clearRxFifo();

void clearTxFifo();

void UhfUartDataRst();

int Um7_SendAndRecv(unsigned char cmd,unsigned char *sendbuf,unsigned char sendlen,unsigned char *recvbuf,unsigned char *recvlen);

int GetUm7Fw(unsigned char *version);

int GetUm7Hw(unsigned char *version);

int GetUm7Rssi(unsigned char * sum);

int readmore(unsigned char* epc);

int readerID(unsigned char* epc);

int readerTemp(unsigned char* epc,float *tempOut);

int readone(unsigned char* epc);

int stopReadmore();

int GetTxPower(unsigned char *sum);

extern int HeadIdx,TailIdx;
extern int uhf_uart_fd;

#endif
