//accept a move input and combine cells. no need to generate a random cell
void run2048(int puzzle[4][4], int dir) {
    if(dir==0){        //left
        for(int i = 0; i < 4; i++){
            int last_combined_col = -1;
            for(int j = 0; j < 4; j++){
                if(puzzle[i][j] == 0){
                    for(int k = j-1; k >=0; k--){
                        if(puzzle[i][k] == 0){
                            puzzle[i][k] = puzzle[i][k+1];
                            puzzle[i][k] = 0;
                        }
                        if(k+1 != last_combined_col){
                            if(puzzle[i][k] == puzzle[i][k+1]){
                                puzzle[i][k]*=2;
                                puzzle[i][k+1] = 0;
                                last_combined_col = k;
                            }
                        }
                    }
                }
            }
        }
    }
    else if(dir==1){   //up
        for(int j = 0; j < 4; j++){
            int last_combined_row = -1;
            for(int i = 0; i < 4; i++){
                if(puzzle[i][j] != 0){
                    for(int k = i-1; k >= 0; k--){
                        if(puzzle[k][j] == 0){
                            puzzle[k][j] = puzzle[k+1][j];
                            puzzle[k+1][j] = 0;
                        }
                        if(k+1 != last_combined_row){
                            if(puzzle[k][j] == puzzle[k+1][j]){
                                puzzle[k][j]*=2;
                                puzzle[k+1][j] = 0;
                                last_combined_row = k;
                            }
                        }
                    }
                }
            }
        }
    }
    else if(dir==2){    //right

    }
    else if(dir==3){    //down

    }

}
