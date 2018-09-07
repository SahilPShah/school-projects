#include "prime.h"

int is_prime(int n){
  //1==prime, 0==not prime
  if(n<=1 && n>=0){return 0;}//1 and 0 are not prime numbers
  else if(n%2==0 && n>2){return 0;}
  else{
      int i;
      for(i=3;i<n/2;i+=2) //increment by 2 because need to check which odd numbers
      {
          if(n%i==0){return 0;}
      }
  }
  return 1;
}
