#include <stdlib.h>
#include <stdio.h>
/*
*Sahil Shah
*sahils2
*This program will compute the semiprimes in the interval [a,b]
*a semiprime is a number whose factor pairs are all prime
*last tested: 10/5/17---passes all test cases on wiki
*committed to svn on 10/5/17 @ 12:16 pm
*/

int is_prime(int number);
int print_semiprimes(int a, int b);


int main(){
   int a, b;
   printf("Input two numbers: ");
   scanf("%d %d", &a, &b);
   if( a <= 0 || b <= 0 ){
     printf("Inputs should be positive integers\n");
     return 1;
 }

   if( a > b ){
     printf("The first number should be smaller than or equal to the second number\n");
     return 1;
   }

   // TODO: call the print_semiprimes function to print semiprimes in [a,b] (including a and b)

   print_semiprimes(a,b); //call print_semiprimes

   return 0;
}


/*
 * TODO: implement this function to check the number is prime or not.
 * Input    : a number
 * Return   : 0 if the number is not prime, else 1
 */
int is_prime(int n)
{
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


/*
 * TODO: implement this function to print all semiprimes in [a,b] (including a, b).
 * Input   : a, b (a should be smaller than or equal to b)
 * Return  : 0 if there is no semiprime in [a,b], else 1
 */
int print_semiprimes(int a, int b)
{
    int areSemiprimes = 0;//if there are semiprimes in [a,b], make this 1
    int n,k;
    int counter = 0; //count how many prime factor pairs a number,n, has
    for(n=a;n<=b;n++)
    {
        counter = 0;//reset the counter
        for(k=2;k<n-1;k++)
        {
            //checking if k is prime and if k is a factor of n
            if(is_prime(k)==1 && n%k==0)
            {
                //checking if n/k is a prime number (given that k is a factor of n)
                if(is_prime(n/k) == 1)
                {
                    counter++;//count how many prime pairs a number has
                    areSemiprimes = 1;//set to one because the number is a semiprime
                    if(counter == 1){printf("%d ", n);} //only print number once if there are multiple prime factor pairs
                }
            }
        }
    }
    printf("\n");//newline
    return areSemiprimes;
}
