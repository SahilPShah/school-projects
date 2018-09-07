#include "sudoku.h"
#include <stdbool.h>
//Sahil Shah
//sahils2
//this file contains functions that solve the sudoku puzzle
/*function:     is_val_in_row(...): checks if the desired value is in the specified row
                is_val_in_col(...): checks if the desired value is in the specified column
                is_val_in_3x3_zone(...): checks if the desired value is in the specified 3x3 zone
                is_val_valid(...):using the functions described above, see if the value can be inputted
                                  into (i,j).
                sudokuFull(...): checks if the sudoku board is full
                solve_sudoku(...): solves the sudoku puzzle using a backtrackig algorithm



*/
//-------------------------------------------------------------------------------------------------
// Start here to work on your MP7
//-------------------------------------------------------------------------------------------------
// You are free to declare any private functions if needed.

// Function: is_val_in_row
//if val is in row == 1
//if val is not in row == 0
// Return true if "val" already existed in ith row of array sudoku.
int is_val_in_row(const int val, const int i, const int sudoku[9][9]) {

  assert(i>=0 && i<9);

  // BEG TODO
  int cell;//loop variables to check each cell in the row
  for(cell=0;cell<9;cell++){
      if(val == sudoku[i][cell])
        return 1;
  }

  return 0;
  // END TODO
}

// Function: is_val_in_col
//if val is in col == 1
//if val is not in col == 0
// Return true if "val" already existed in jth column of array sudoku.
int is_val_in_col(const int val, const int j, const int sudoku[9][9]) {
  assert(j>=0 && j<9);
  int cell;//loop variables to check each cell in the row
  for(cell=0;cell<9;cell++){
      if(val == sudoku[cell][j])
        return 1;
  }
  return 0;
}

// Function: is_val_in_3x3_zone
//if value is in 3x3 == 1
//if value is not in 3x3 == 0
// Return true if val already existed in the 3x3 zone corresponding to (i, j)
int is_val_in_3x3_zone(const int val, const int i, const int j, const int sudoku[9][9]) {

  assert(i>=0 && i<9);
//starting row and column indeces for the 3x3 zone
int row_start = i - (i%3);
int col_start = j - (j%3);
int row,col;
 //must start loop at the top left corner of the 3x3 square.
 //reset col_start everytime a row has been cols_checked
 for(row=0;row<3;row++){
     for(col=0;col<3;col++){
         if(sudoku[row+row_start][col+col_start]==val)
            return 1;
     }
 }
  return 0;
}

// Function: is_val_valid
//if value can be inputed == 1
//if value cannot be inputted == 0
// Return true if the val is can be filled in the given entry.
int is_val_valid(const int val, const int i, const int j, const int sudoku[9][9]) {

  assert(i>=0 && i<9 && j>=0 && j<9);
  if(is_val_in_3x3_zone(val,i,j,sudoku)==1)
    return 0;
  if(is_val_in_row(val,i,sudoku)==1)
    return 0;
  if(is_val_in_col(val,j,sudoku) == 1)
    return 0;
  return 1;
}

//check if the board is full or not
//if full, @return true
//if there are empty spaces, @return false
bool sudokuFull(int sudoku[9][9]){
  int i,j;//loops variables
  for(i=0;i<9;i++){
    for(j=0;j<9;j++){
      if(sudoku[i][j] == 0)
        return false;
    }
  }
  return true;
}
// Procedure: solve_sudoku
// Solve the given sudoku instance.
int solve_sudoku(int sudoku[9][9]) {
int i,j;//location of the empty
int row,col;//loop variables
if(sudokuFull(sudoku))
  return 1;//there are no more empty cells
else{
  for(row=0;row<9;row++){
    for(col=0;col<9;col++){
      if(sudoku[row][col]==0){
        i=row;
        j=col;
        goto emptyFound;//branch if 'emptyFound' if an empty cell has been found --> functions as a double break

      }
    }
  }
}
emptyFound:
int val;//possible values to be used in each sudoku cell
for(val=1;val<=9;val++){
    if(is_val_valid(val, i, j, sudoku)==1){//check if the value is valid
        sudoku[i][j] = val; //put the value into (i,j)
        if(solve_sudoku(sudoku)==1){return 1;}//check if the permutation of that value in that position works
        sudoku[i][j] = 0;//backtracking -- if program doesnt return, the val did not work, empty that cell
      }
    }
  return 0;
}

// Procedure: print_sudoku
void print_sudoku(int sudoku[9][9])
{
  int i, j;
  for(i=0; i<9; i++) {
    for(j=0; j<9; j++) {
      printf("%2d", sudoku[i][j]);
    }
    printf("\n");
  }
}

// Procedure: parse_sudoku
void parse_sudoku(const char fpath[], int sudoku[9][9]) {
  FILE *reader = fopen(fpath, "r");
  assert(reader != NULL);
  int i, j;
  for(i=0; i<9; i++) {
    for(j=0; j<9; j++) {
      fscanf(reader, "%d", &sudoku[i][j]);
    }
  }
  fclose(reader);
}
