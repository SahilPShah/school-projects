
/*Sahil Shah
*sahils2
*This file contains functions that implement the game of life
*function: countLiveNeighbor(...): make sure that the specified row and coloumn is not
																	 at a corner or on the edge. If it was, check the 3 or 5 neighbors respectively
																	 and count the live ones. If the row/col passed is not in a corner or on the edge
																	 then check the 8 neighbors and count the live ones
*function: updateBoard(...): for each cell in the array, check to see how many live neighbors it has and decide whether
														 or not the cell lives or dies
*function: aliveStable(...): using the same logic as updateBoard, check if each cell WOULD change. If so, the game is not
														 stable. If the game WOULD NOT change, the game is stable
*/
/*
 * countLiveNeighbor
 * Inputs:
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * row: the row of the cell that needs to count alive neighbors.
 * col: the col of the cell that needs to count alive neighbors.
 * Output:
 * return the number of alive neighbors. There are at most eight neighbors.
 * Pay attention for the edge and corner cells, they have less neighbors.
 */
int countLiveNeighbor(int* board, int boardRowSize, int boardColSize, int row, int col){
//board[i*COL+j] == board[i][j]
int liveNeighbors=0;//live neighbors variable
if(row == 0 && col == 0){//upper left corner
	if(board[(row+1)*boardColSize + col] == 1)
		liveNeighbors++;
	if(board[(row+1)*boardColSize + (col+1)] == 1)
		liveNeighbors++;
	if(board[(row)*boardColSize + (col+1)] == 1)
		liveNeighbors++;
}
else if(row == 0 && col == boardColSize-1){//upper right corner
	if(board[(row+1)*boardColSize + col] == 1)
		liveNeighbors++;
	if(board[(row+1)*boardColSize + (col-1)] == 1)
		liveNeighbors++;
	if(board[(row)*boardColSize + (col-1)] == 1)
		liveNeighbors++;
}
else if(row == boardRowSize-1 && col == 0){//bottom left corner
	if(board[(row-1)*boardColSize + (col)] == 1)
		liveNeighbors++;
	if(board[(row-1)*boardColSize + (col+1)] == 1)
		liveNeighbors++;
	if(board[(row)*boardColSize + (col+1)] == 1)
		liveNeighbors++;
}
else if(row == boardRowSize && col == boardRowSize){//bottom right corner
	if(board[(row)*boardColSize + (col-1)] == 1)
		liveNeighbors++;
	if(board[(row-1)*boardColSize + (col-1)] == 1)
		liveNeighbors++;
	if(board[(row-1)*boardColSize + (col)] == 1)
		liveNeighbors++;
}
else if(row == 0 && col != 0 && col != boardColSize-1){//upper edge
	if(board[(row)*boardColSize + (col-1)] == 1)
		liveNeighbors++;
	if(board[(row+1)*boardColSize + (col-1)] == 1)
		liveNeighbors++;
	if(board[(row+1)*boardColSize + (col)] == 1)
		liveNeighbors++;
	if(board[(row+1)*boardColSize + (col+1)] == 1)
		liveNeighbors++;
	if(board[(row)*boardColSize + (col+1)] == 1)
		liveNeighbors++;
}
else if(row == boardRowSize-1 && col != 0 && col!= boardColSize-1){//lower edge
	if(board[(row)*boardColSize + (col-1)] == 1)
		liveNeighbors++;
	if(board[(row)*boardColSize + (col+1)] == 1)
		liveNeighbors++;
	if(board[(row-1)*boardColSize + (col-1)] == 1)
		liveNeighbors++;
	if(board[(row-1)*boardColSize + (col)] == 1)
		liveNeighbors++;
	if(board[(row-1)*boardColSize + (col+1)] == 1)
		liveNeighbors++;
}
else if(col == 0 && col != 0 && col != boardRowSize-1){//left edge
	if(board[(row-1)*boardColSize + (col)] == 1)
		liveNeighbors++;
	if(board[(row-1)*boardColSize + (col+1)] == 1)
		liveNeighbors++;
	if(board[(row)*boardColSize + (col+1)] == 1)
		liveNeighbors++;
	if(board[(row+1)*boardColSize + (col)] == 1)
		liveNeighbors++;
	if(board[(row+1)*boardColSize + (col+1)] == 1)
		liveNeighbors++;
}
else if(col == boardColSize-1 && row!=0 && row!=boardRowSize-1){//right edge
	if(board[(row-1)*boardColSize + (col)] == 1)
		liveNeighbors++;
	if(board[(row-1)*boardColSize + (col-1)] == 1)
		liveNeighbors++;
	if(board[(row)*boardColSize + (col-1)] == 1)
		liveNeighbors++;
	if(board[(row+1)*boardColSize + (col)] == 1)
		liveNeighbors++;
	if(board[(row+1)*boardColSize + (col-1)] == 1)
		liveNeighbors++;
}
else{//middle case -- not on any of the edges
	if(board[(row-1)*boardColSize + (col-1)] == 1)
		liveNeighbors++;
	if(board[(row-1)*boardColSize + (col)] == 1)
		liveNeighbors++;
	if(board[(row-1)*boardColSize + (col+1)] == 1)
		liveNeighbors++;
	if(board[(row)*boardColSize + (col-1)] == 1)
		liveNeighbors++;
	if(board[(row)*boardColSize + (col+1)] == 1)
		liveNeighbors++;
	if(board[(row+1)*boardColSize + (col-1)] == 1)
		liveNeighbors++;
	if(board[(row+1)*boardColSize + (col)] == 1)
		liveNeighbors++;
	if(board[(row+1)*boardColSize + (col+1)] == 1)
		liveNeighbors++;
}
return liveNeighbors;
}
/*
 * Update the game board to the next step.
 * Input:
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * Output: board is updated with new values for next step.
 */
void updateBoard(int* board, int boardRowSize, int boardColSize) {
	int row,col;
	int update_board[boardRowSize*boardColSize]; //new updated board that is the same size as the previous array
	for(row=0;row<boardRowSize;row++){
		for(col=0;col<boardColSize;col++){
			update_board[row*boardColSize + col] = board[row*boardColSize + col];
		}
	}
	int liveNeighbors=0;//how many live neighbors a cell has
	for(row=0;row<boardRowSize;row++){
		for(col=0;col<boardColSize;col++){
				//loop through the array and check how many live neighbors each cell has
				liveNeighbors = countLiveNeighbor(board, boardRowSize, boardColSize, row, col);
				if(board[row*boardColSize + col] == 1){//cell was alive
					if(liveNeighbors == 2 || liveNeighbors == 3)
						update_board[row*boardColSize + col] = 1; //stay alive
					else if(liveNeighbors < 2 || liveNeighbors > 3)
						update_board[row*boardColSize + col] = 0; //dies
				}
				else if(board[row*boardColSize + col] == 0){//cell was dead
					if(liveNeighbors == 3)
						update_board[row*boardColSize + col] = 1; //becomes alive 
					else
						update_board[row*boardColSize + col] = 0; //stays dead;
				}
		}
	}
	for(row=0;row<boardRowSize;row++){
		for(col=0;col<boardColSize;col++){
			board[row*boardColSize + col] = update_board[row*boardColSize + col];//take value in update_board and put it into board
		}
	}
}

/*
 * aliveStable
 * Checks if the alive cells stay the same for next step
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * Output: return 1 if the alive cells for next step is exactly the same with
 * current step or there is no alive cells at all.
 * return 0 if the alive cells change for the next step.
 */
int aliveStable(int* board, int boardRowSize, int boardColSize){
	int row,col;
	int liveNeighbors=0;//how many live neighbors a cell has
	for(row=0;row<boardRowSize;row++){
		for(col=0;col<boardColSize;col++){
				//loop through the array and check how many live neighbors each cell has
				liveNeighbors = countLiveNeighbor(board, boardRowSize, boardColSize, row, col);
				if(board[row*boardColSize + col] == 1){//cell was alive
					if(liveNeighbors != 2 && liveNeighbors != 3)
						return 0;
				}
				else if(board[row*boardColSize + col] == 0){//cell was dead
					if(liveNeighbors == 3)
						return 0;
				}
		}
	}
return 1;

}
