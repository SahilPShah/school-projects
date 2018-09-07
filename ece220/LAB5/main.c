/* Code to simulate rolling three six-sided dice (D6)
 * User first types in seed value
 * Use seed value as argument to srand()
 * Call roll_three to generate three integers, 1-6
 * Print result "%d %d %d "
 * If triple, print "Triple!\n"
 * If it is not a triple but it is a dobule, print "Double!\n"
 * Otherwise print "\n"
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include "dice.h"

 int main(){
     int one, two, three, seed, count=0;
     printf("Please input a seed: ");
     scanf("%d", &seed);
     srand(seed);
     roll_three(&one, &two, &three);
     if(one == two)
        count++;
     if(one == three)
        count++;
     if(two == three)
        count++;
     printf("%d %d %d", one, two, three);
     if(count == 1)//if there was a double, there would be 1 match therefore count == 1
        printf(" Double!");
     if(count == 3)//if there is a triple, there would be 3 matches so count == 3
        printf(" Triple!");

     printf("\n");
     return 0;
 }
