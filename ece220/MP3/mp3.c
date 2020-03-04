//Sahil Shah
//sahils2
//this program will compute the coefficients for a specific row in Pascal's triangle

#include <stdio.h>
#include <stdlib.h>
int main()
{
    unsigned long result,n,k; //recursive coefficient variable//current coefficient number//
    unsigned long i; //this variable forces the result to be a fraction so must use a data type that allows decimals
    printf("Enter a row index: ");
    scanf("%lu",&n); //user inputs row number
    for(k=0;k<=n;k++)//find the kth coefficient in the nth row
    {
        result = 1;
        for(i=1;i<=k;i++)//recursive loop to calculate kth coefficient
        {
            result = result * (n+1-i); //computation
	        result /= i;
        }
        printf("%lu ", result);
    }
    printf("\n");``
    return 0;
