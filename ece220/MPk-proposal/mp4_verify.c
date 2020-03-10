#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mp4_verify.h"

/* This is a what a proposed MP4 verification program could look like
* Author: Sahil Shah
* RE: Emily Moog's MP3,MP4 combination proposal.
*/


int is_prime(int n){
    int i;
    if((n<=1 && n>=0) || (n%2==0 && n>2)){
        return 0;
    }
    for(i=3;i<n/2;i+=2)
    {
        if(n%i==0){
            return 0;
        }
    }
    return 1;
}

int get_semiprimes(int a, int b, int* array){
    int num_semiprimes = 0;
    int areSemiprimes = 0;
    int n,k;
    int counter = 0; //count how many prime factor pairs a number,n, has
    for(n=a;n<=b;n++)
    {
        counter = 0;
        for(k=2;k<n-1;k++)
        {
            //checking if k is prime and if k is a factor of +
            if(is_prime(k)==1 && n%k==0)
            {
                //checking if n/k is a prime number (given that k is a factor of n)
                if(is_prime(n/k) == 1)
                {
                    counter++;//count how many prime pairs a number has
                    areSemiprimes = 1;//set to one because the number is a semiprime
                    if(counter == 1){
                        array[num_semiprimes] = n;
                        num_semiprimes++;
                    } //only print number once if there are multiple prime factor pairs
                }
            }
        }
    }
    array[b-a+1] = num_semiprimes;
    return areSemiprimes;
}
//probably accessing something outside the bounds of the semiprime_array alloc'd here.
//invalid memory access probably occurs in get_semiprimes

int main(int argc, char** argv){
    //int i;
    if(argc != 3){
        printf("proper usage: ./mp4_verify <a> <b>\n");
        printf("Aborting...\n");
        return 0;
    }
    int start = atoi(argv[1]);
    int end = atoi(argv[2]);
    int* semiprime_array = (int*) malloc(sizeof(int) * (end-start+2));
    if(semiprime_array == NULL){
        printf("ERROR: malloc failed");
        return 0;
    }
    if(!get_semiprimes(start, end, semiprime_array)){
        printf("No semiprimes detected");
        return 0;
    }
    printf("Number of Semiprimes within range: %d\n", semiprime_array[end-start+1]);
    for(int i = 0; i < semiprime_array[end-start+1]; i++){
        printf("%d ", semiprime_array[i]);
    }
    printf("\n");
    //run verification here
    return 1;
}
