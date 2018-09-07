
/*
 *last committed: 10/12/17 @ 9:38pm
 *Sahil Shah
 *sahils2
 *funtion:
 *      set_seed: get user input for the seed and pass it to the srand() function
 *      start_game: use the rand() function to create the solution and store into the pointers
                    passed to the function and the static variables
 *      make_guess: determine if input is valid first. Then check the nth guess with the nth solution
                    for any perfect matches. Then check all solutions without a perfect match against
                    each solution to look for misplaced matches. Increment the guess_number with each guess.
                    Print out the guess number and the number of perfect matches and misplaced  matches the
                    user has after each guess.
 * The functions that you must write are defined in the header file.
 * Blank function prototypes with explanatory headers are provided
 * in this file to help you get started.
 */



#include <stdio.h>
#include <stdlib.h>

#include "prog5.h"


/*
 * You will need to keep track of the solution code using file scope
 * variables as well as the guess number.
 */

static int guess_number;
static int solution1;
static int solution2;
static int solution3;
static int solution4;



/*
 *PASSES TEST
 * set_seed -- This function sets the seed value for pseudorandom
 * number generation based on the number the user types.
 * The input entered by the user is already stored in the string seed_str by the code in main.c.
 * This function should use the function sscanf to find the integer seed value from the
 * string seed_str, then initialize the random number generator by calling srand with the integer
 * seed value. To be valid, exactly one integer must be entered by the user, anything else is invalid.
 * INPUTS: seed_str -- a string (array of characters) entered by the user, containing the random seed
 * OUTPUTS: none
 * RETURN VALUE: 0 if the given string is invalid (string contains anything
 *               other than a single integer), or 1 if string is valid (contains one integer)
 * SIDE EFFECTS: initializes pseudo-random number generation using the function srand. Prints "set_seed: invalid seed\n"
 *               if string is invalid. Prints nothing if it is valid.
 */
int set_seed (const char seed_str[])
{
    int seed;
    char post[2];
    if(sscanf(seed_str, "%d%1s", &seed, post) != 1){
        printf("Invalid Seed: please enter only a number\n");
        return 0;
    }//check if an integer was inputted
     //by checking how many characters were inputted
    else
        srand(seed);
    return 1;
}


/*
 *PASSES TEST
 * start_game -- This function is called by main.c after set_seed but before the user makes guesses.
 *               This function creates the four solution numbers using the approach
 *               described in the wiki specification (using rand())
 *               The four solution numbers should be stored in the static variables defined above.
 *               The values at the pointers should also be set to the solution numbers.
 *               The guess_number should be initialized to 1 (to indicate the first guess)
 * INPUTS: none
 * OUTPUTS: *one -- the first solution value (between 1 and 6)
 *          *two -- the second solution value (between 1 and 6)
 *          *three -- the third solution value (between 1 and 6)
 *          *four -- the fourth solution value (between 1 and 6)
 * RETURN VALUE: none
 * SIDE EFFECTS: records the solution in the static solution variables for use by make_guess, set guess_number
 */
void start_game (int* one, int* two, int* three, int* four)
{
    *one = rand()%6+1;
    *two = rand()%6+1;
    *three = rand()%6+1;
    *four = rand()%6+1;
    solution1 = *one;
    solution2 = *two;
    solution3 = *three;
    solution4 = *four;
    guess_number = 1;
}

/*
 *KINDA PASSES TEST
 * make_guess -- This function is called by main.c after the user types in a guess.
 *               The guess is stored in the string guess_str.
 *               The function must calculate the number of perfect and misplaced matches
 *               for a guess, given the solution recorded earlier by start_game
 *               The guess must be valid (contain only 4 integers, within the range 1-6). If it is valid
 *               the number of correct and incorrect matches should be printed, using the following format
 *               "With guess %d, you got %d perfect matches and %d misplaced matches.\n"
 *               If valid, the guess_number should be incremented.
 *               If invalid, the error message "make_guess: invalid guess\n" should be printed and 0 returned.
 *               For an invalid guess, the guess_number is not incremented.
 * INPUTS: guess_str -- a string consisting of the guess typed by the user
 * OUTPUTS: the following are only valid if the function returns 1 (A valid guess)
 *          *one -- the first guess value (between 1 and 6)
 *          *two -- the second guess value (between 1 and 6)
 *          *three -- the third guess value (between 1 and 6)
 *          *four -- the fourth color value (between 1 and 6)
 * RETURN VALUE: 1 if the guess string is valid (the guess contains exactly four
 *               numbers between 1 and 6), or 0 if it is invalid
 * SIDE EFFECTS: prints (using printf) the number of matches found and increments guess_number(valid guess)
 *               or an error message (invalid guess)
 *               (NOTE: the output format MUST MATCH EXACTLY, check the wiki writeup)
 */
int make_guess (const char guess_str[], int* one, int* two,
	    int* three, int* four)
{
//  One thing you will need to read four integers from the string guess_str, using a process
//  similar to set_seed
//  The statement, given char post[2]; and four integers w,x,y,z,
//  sscanf (guess_str, "%d%d%d%d%1s", &w, &x, &y, &z, post)
//  will read four integers from guess_str into the integers and read anything else present into post
//  The return value of sscanf indicates the number of items sucessfully read from the string.
//  You should check that exactly four integers were sucessfully read.
//  You should then check if the 4 integers are between 1-6. If so, it is a valid guess
//  Otherwise, it is invalid.
//  Feel free to use this sscanf statement, delete these comments, and modify the return statement as needed

//0 if no perfect match, 1 if perfect match
int perfect1=0,perfect2=0,perfect3=0,perfect4=0; //keep track of where the perfect matches occur
int matchW = 0, matchX = 0, matchY = 0, matchZ = 0;//keep track of guesses that are misplaced or perfect matches
                                                  //so they arent counted twice
int w=-1,x=-1,y=-1,z=-1;//error check, if these vars, are not within the range of [1,6] after the input.its invalid
char post[2];//to ensure that no characaters are typed
int num_perf =0;
int num_misplaced =0;
if((sscanf(guess_str, "%d%d%d%d%1s", &w, &x, &y, &z, post) != 4) || (w<1 || w >6) || (x<1 || x>6) || (y<1 || y>6) || (z<1 || z>6))
{
    printf("Invalid Guess: Input 4 numbers between 1 and 6\n");
    return 0;
}
//pass the value of the guesses into the pointers passed to the function

*one = w;
*two = x;
*three = y;
*four = z;
//check for perfect matches
if(w == solution1)
{
    num_perf++;
    perfect1 = 1;
    matchW = 1;
}
if(x == solution2)
{
    num_perf++;
    perfect2 = 1;
    matchX = 1;
}
if(y == solution3)
{
    num_perf++;
    perfect3 = 1;
    matchY = 1;
}
if(z == solution4)
{
    num_perf++;
    perfect4 = 1;
    matchZ = 1;
}
//check misplaced matches
if(perfect1 != 1)//check solution1 with all inputs except W
{
    if(solution1 == x && matchX != 1){
        num_misplaced++;
        matchX = 1;
    }
    else if(solution1 == y && matchY !=1){
        num_misplaced++;
        matchY = 1;
    }
    else if(solution1 == z && matchZ != 1){
        num_misplaced++;
        matchZ = 1;
    }
}
if(perfect2 != 1)//check solution2 with all inputs except X
{
    if(solution2 == w && matchW !=1){
        num_misplaced++;
        matchW = 1;
    }
    else if(solution2 == y && matchY !=1){
        num_misplaced++;
        matchY = 1;
    }
    else if(solution2 == z && matchZ != 1){
        num_misplaced++;
        matchZ= 1;
    }
}
if(perfect3 != 1)//check solution3 with all inputs except Y
{
    if(solution3 == x && matchX !=1){
        num_misplaced++;
        matchX = 1;
    }
    else if(solution3 == w && matchW !=1){
        num_misplaced++;
        matchW = 1;
    }
    else if(solution3 == z && matchZ != 1){
        num_misplaced++;
        matchZ = 1;
    }
}
if(perfect4 != 1)//check solution4 with all inputs except Z
{
    if(solution4 == x && matchX !=1){
        num_misplaced++;
        matchX = 1;
    }
    else if(solution4 == y && matchY !=1){
        num_misplaced++;
        matchY = 1;
    }
    else if(solution4 == w && matchW != 1){
        num_misplaced++;
        matchW = 1;
    }
}
    printf("With guess number %d ", guess_number++);
    printf("you have %d perfect matches and %d misplaced matches\n", num_perf, num_misplaced);
    return 1;
}
