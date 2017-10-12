#include <stdio.h>

#define MAIN (0)

int sum(int num_numbers, int *numbers) {
  int sum=0;
  for (int i = 0; i < num_numbers; i++) {
    sum += numbers[i];
  }
  return sum;
}

#if MAIN
int main(){
  int n[] = {1,2,3};
  printf("%d\n",our_function(3,n));
}
#endif 
