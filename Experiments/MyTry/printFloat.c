#include "printFloat.h"

#define MAIN (0)

void printFloat(float num){
  printf("C:printInt : %f\n",num);

}

#if MAIN
int main(int argc,char **argv){
  if (argc == 2){
    printFloat(atof(argv[1]));
  }
  else{
    printf("usage : $ ./a.out <int>\n");
  }
  return 0;
}
#endif 
