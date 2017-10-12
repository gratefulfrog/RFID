#include "printInt.h"

#define MAIN (0)

void printInt(int num){
  printf("C:printInt : %d\n",num);

}

int returnIntPlusOne(int x){
  return x + 1;
}

#if MAIN
int main(int argc,char **argv){
  if (argc == 2){
    printInt(atoi(argv[1]));
    printInt(returnIntPlusOne(atoi(argv[1])));
  }
  else{
    printf("usage : $ ./a.out <int>\n");
  }
  return 0;
}
#endif 
