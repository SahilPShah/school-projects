#include <stdio.h>
#include <stdlib.h>
#include "maze.h"
//Sahil Shah
//sahils2
//this program is designed to create and solve a maze given from a specific file

/*
 * createMaze -- Creates and fills a maze structure from the given file
 * INPUTS:       fileName - character array containing the name of the maze file
 * OUTPUTS:      None
 * RETURN:       A filled maze structure that represents the contents of the input file
 * SIDE EFFECTS: None
 */
maze_t * createMaze(char * fileName)
{
    // Your code here. Make sure to replace following line with your own code.
    FILE *mazefile = fopen(fileName, "r");
    if(mazefile==NULL)
    {
      printf("NULL");
      return NULL;
    }
    maze_t *maze = malloc(sizeof(maze_t));
    int rows,cols;
    fscanf(mazefile,"%d %d", &rows, &cols);
    if(maze == NULL){
      printf("maze pointer is NULL");
      return NULL;
    }
    maze->height = rows;
    maze->width = cols;
    maze->cells=malloc(rows*sizeof(char*));//allocate memory for each row
    int i,j;//loop variables
    int val = fgetc(mazefile);//values from the file that make up the maze
    for(i=0;i<rows;i++){
      maze->cells[i] = malloc(cols*sizeof(char));//allocate memory for each individual column
    }
    //inititialize the array
    for(i=0;i<rows;i++){                        //end check
        for(j=0;j<cols;j++){
            val = fgetc(mazefile);
            maze->cells[i][j] = val;
            if(val==START){
                maze->startRow=i;
                maze->startColumn=j;
            }
            else if(val==END){
                maze->endRow=i;
                maze->endColumn=j;              //end check
            }
        }
        fgetc(mazefile);//remove the newline char from the buffer
    }
    fclose(mazefile);
    return maze;//return the pointer to the struct
}

/*
 * destroyMaze -- Frees all memory associated with the maze structure, including the structure itself
 * INPUTS:        maze -- pointer to maze structure that contains all necessary information
 * OUTPUTS:       None
 * RETURN:        None
 * SIDE EFFECTS:  All memory that has been allocated for the maze is freed
 */
void destroyMaze(maze_t * maze)
{
    // Your code here.
    int i;
    //free the column
    for(i=0;i<maze->height;i++){
      free(maze->cells[i]);
    }
    free(maze->cells);//free the rows
    free(maze);//free the struct
}

/*
 * printMaze --  Prints out the maze in a human readable format (should look like examples)
 * INPUTS:       maze -- pointer to maze structure that contains all necessary information
 *               width -- width of the maze
 *               height -- height of the maze
 * OUTPUTS:      None
 * RETURN:       None
 * SIDE EFFECTS: Prints the maze to the console
 */
void printMaze(maze_t * maze)
{
    // Your code here.
    int i,j;//loop variables
    //loop thru the 2D array
    for(i=0;i<maze->height;i++){
      for(j=0;j<maze->width;j++){
        printf("%c", maze->cells[i][j]);
      }
      printf("\n");
    }
}

/*
 * solveMazeManhattanDFS -- recursively solves the maze using depth first search,
 * INPUTS:               maze -- pointer to maze structure with all necessary maze information
 *                       col -- the column of the cell currently beinging visited within the maze
 *                       row -- the row of the cell currently being visited within the maze
 * OUTPUTS:              None
 * RETURNS:              0 if the maze is unsolvable, 1 if it is solved
 * SIDE EFFECTS:         Marks maze cells as visited or part of the solution path
 */
int solveMazeManhattanDFS(maze_t * maze, int col, int row)//NOTICE: it is col,row NOT row,col
{
    if(row<0 || row>=maze->height || col<0 || col>=maze->width)//if the given row/col is out of bounds
        return 0;
    if(row==maze->endRow && col==maze->endColumn){
      maze->cells[maze->startRow][maze->startColumn] = START;
      return 1;
    }
    if(maze->cells[row][col] != ' ' && maze->cells[row][col] != START)// && maze->cells[row][col] != END)
        return 0;
    maze->cells[row][col] = PATH;//set as part of solution path
    if(solveMazeManhattanDFS(maze,col-1,row)==1)//check the cell to the left
        return 1;
    if(solveMazeManhattanDFS(maze,col+1,row)==1)//check the cell to the right
       return 1;
    if(solveMazeManhattanDFS(maze,col,row-1)==1)//check the cell above
        return 1;
    if(solveMazeManhattanDFS(maze,col,row+1)==1)//check the cell below
        return 1;
    maze->cells[row][col] = VISITED;//backtrack
    return 0;
}
