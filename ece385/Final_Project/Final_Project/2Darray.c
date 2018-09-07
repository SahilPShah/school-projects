#include <stdlib.h>
#include <stdio.h>

/*
*@author, Sahil Shah
*The purpose of this script is to convert a 1D array with Row-maor addressing to a 2D array for addressing convenience
*/

int main(){
    FILE *in_file = fopen("NewSprites/Megaman_facing_left.txt", "r");
    FILE *out_file = fopen("NewSprites/2D_A.txt", "w");

    const int HEIGHT = 60;
    const int WIDTH = 60;
    int i,j;
    //change the height and width as needed
    if(in_file == NULL){
        printf("ERROR! -- infile");
        return 1;
    }
    if(out_file == NULL){
        printf("ERROR! -- outfile");
        return 1;
    }
    for(i = 0; i < HEIGHT; i++){
        for(j =0; j < WIDTH; j++){
            char character = fgetc(in_file);
            char newline = fgetc(in_file);

            if(j!=WIDTH-1)
                if(j==0)
                    fprintf(out_file, "'{%c, ", character);
                else
                    fprintf(out_file, "%c, ", character);
            else if(i != HEIGHT - 1)
                fprintf(out_file, "%c},", character);
            else
                fprintf(out_file, "%c}", character);
        }
        fprintf(out_file, "\n");
    }

    fclose(in_file);
    fclose(out_file);
    return 0;
}
