#include "little_test.h"

#define TEST

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

#ifdef TEST
#define INPUT (1)
void wiringPiSetup (){
}
void pinMode(int x, int y){
}
int digitalRead(int x){
  return 0;
}
#endif

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

  printf("Version Fw: ");
  nbBytes = GetUm7Fw(uart_data);
  printBytesN(uart_data,nbBytes);
  sleep(0.5);

  printf("Version Hw: ");
  nbBytes = GetUm7Hw(uart_data);
  printBytesN(uart_data,nbBytes);
  sleep(0.5);
  
  printf("TxPower: ");
  nbBytes = GetTxPower(uart_data);
  printBytesN(uart_data,nbBytes);
  sleep(0.5);
  
  printf("Reader ID: ");
  nbBytes = readerID(uart_data);
  printBytesN(uart_data,nbBytes);
  sleep(0.5);
  
  printf("RSSI: ");
  nbBytes = GetUm7Rssi(uart_data);
  printBytesN(uart_data,nbBytes);
  sleep(0.5);
  
  float temp =0.0;
  nbBytes = readerTemp(uart_data,&temp);
  printf("temperature : %f\n",temp);
  sleep(0.5);

  while(digitalRead(7)){

    nbBytes  = readmore(uart_data);
    if(nbBytes){
          printf("Tag: ");
	  printBytesN(uart_data,nbBytes);
	  printf("\n");
	  sleep(0.5);
    }
    if (nbBytes){
      //printf("stopReadmore: ");
      nbBytes  = stopReadmore(uart_data);
      //printBytesN(uart_data,nbBytes);
      sleep(0.5);
    }
  }
  close(uhf_uart_fd);	
  return 0;
}

