#include <stdio.h>
#include "prime.h"

int main(){
  // Write the code to take a number n from user and print all the prime numbers between 1 and n

 int n;
 printf("Please input a character ");
 scanf("%d",&n);
 //loop through all numbers <=30 and print all the prime ones
 int i;
 for(i=1;i<=n;i++)
 {
     //print if the number is prime
     if(is_prime(i)==1){
         printf("%d, ",i);
     }
 }
 printf("\n");
 return 0;
}
