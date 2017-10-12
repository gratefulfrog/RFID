#include "printString.h"

#define MAIN (0)

/* we will not use wide types
void printStringC(wchar_t *ch){
  wprintf(L"C:printStringC : %s\n",ch);
}
*/

void printString(char *ch){
  printf("C:printString : %s\n",ch);

}

#if MAIN
int main(int argc,char **argv){
  if (argc == 2){
    //printStringC((wchar_t*)argv[1]);
    printString(argv[1]);
  }
  else{
    printf("usage : $ ./a.out <char>\n");
  }
  return 0;
}
#endif 
