#include "little_test.h"

#ifdef TEST
#define INPUT (1)
void wiringPiSetup (){
}
void pinMode(int x, int y){
}
int digitalRead(int x){
  return 1;
}
#else
#include <wiringPi.h>
#endif

void printBytes(unsigned char *bytes){
  for (int i=0;i<50;i++){
    printf(" %u", bytes[i]);
  }
  printf("\n");
}

void   printBytesN(unsigned char *bytes, int nbBytes){
  for (int i=0;i<nbBytes;i++){
    printf(" %u", bytes[i]);
  }
  printf("\n");
}

int openPort(){
  uhf_uart_fd = uart_open(PORT, 115200, 8, 1, 'N', 0);
  if(uhf_uart_fd == -1)	{
    printf("connect error result = %d\n", uhf_uart_fd);
  }
  else{
    printf("connect success!\n");
  }
  return uhf_uart_fd;
}

void showFWVersion(){
  int nbBytes;
  unsigned char uart_data[500];

  printf("Version Fw: ");
  nbBytes = GetUm7Fw(uart_data);
  printBytesN(uart_data,nbBytes);
}

void showHWVersion(){
  int nbBytes;
  unsigned char uart_data[500];

  printf("Version Hw: ");
  nbBytes = GetUm7Hw(uart_data);
  printBytesN(uart_data,nbBytes);
}

void showTxPower(){
  int nbBytes;
  unsigned char uart_data[500];

  printf("TxPower: ");
  nbBytes = GetTxPower(uart_data);
  printBytesN(uart_data,nbBytes);
}

void showReaderID(){
  int nbBytes;
  unsigned char uart_data[500];

  printf("Reader ID: ");
  nbBytes = readerID(uart_data);
  printBytesN(uart_data,nbBytes);
}

void showRSSI(){
  int nbBytes;
  unsigned char uart_data[500];

  printf("RSSI: ");
  nbBytes = GetUm7Rssi(uart_data);
  printBytesN(uart_data,nbBytes);
}

void showTemp(){
  unsigned char uart_data[500];

  float temp =0.0;
  readerTemp(uart_data,&temp);
  printf("Temperature : %f\n",temp);
}  

int showReadMore(){
  int nbBytes;
  unsigned char uart_data[500];
  nbBytes  = readmore(uart_data);
  if(nbBytes){
    printf("Tag: ");
    printBytesN(uart_data,nbBytes);
    printf("\n");
  }
  return nbBytes;
}
void doStopReadMore(){
  unsigned char uart_data[500];
  stopReadmore(uart_data);
}
void shutDown(int fd){
  close(fd);
}

#ifdef MAIN
int main(){
  int result, nbBytes;
  int len;
  unsigned char uart_data[500];

  wiringPiSetup () ;
  pinMode (7, INPUT) ;
  
  
  uhf_uart_fd = uart_open(PORT, 115200, 8, 1, 'N', 0);
  if(uhf_uart_fd == -1)	{
    printf("connect error result = %d\n", uhf_uart_fd);
    return 0;
  }
  printf("connect success!\n");

  showFWVersion();
  sleep(0.5);

  showHWVersion();
  sleep(0.5);
  
  showTxPower();
  sleep(0.5);

  showReaderID();
  sleep(0.5);

  showRSSI();
  sleep(0.5);
  
  showTemp();
  sleep(0.5);

  while(1){
    if(digitalRead(7)){
      nbBytes  = readmore(uart_data);
      if(nbBytes){
	printf("Tag: ");
	printBytesN(uart_data,nbBytes);
	printf("\n");
	sleep(0.5);
      }
      if (nbBytes){
	nbBytes  = stopReadmore(uart_data);
	sleep(0.5);
      }
    }
  }
  shutDown(uhf_uart_fd);
  return 0;
}
#endif
