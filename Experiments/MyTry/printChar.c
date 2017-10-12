#include "printChar.h"

#define MAIN (0)

void printChar(char ch){
  printf("C:printChar : %c\n",ch);

}

// we will not use wide types
/*
void printCharC(wchar_t ch){
  printf("C:printCharC : %c\n",ch);

}
*/

#if MAIN
int main(int argc,char **argv){
  if (argc == 2){
    printChar(argv[1][0]);
  }
  else{
    printf("usage : $ ./a.out <char>\n");
  }
  return 0;
}
#endif 
