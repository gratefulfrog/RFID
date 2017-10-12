#ifndef LITTLE_TEST_H
#define LITTLE_TEST_H

#include <time.h>
#include "lib_rfid_driver.h"

extern void printBytes(unsigned char *bytes);
extern void printBytesN(unsigned char *bytes, int nbBytes);
extern int  openPort();
extern void showFWVersion();
extern void showHWVersion();
extern void showTxPower();
extern void showReaderID();
extern void showRSSI();
extern void showTemp();
extern int  showReadMore();
extern void doStopReadMore();
extern void shutDown(int fd);

#endif
