#include "maze.h"
#include <sys/time.h>
//MP COMPLETE
/*
* The constructor of a SquareMaze simply initializes the random number seed that will be used
* to randomly generate the maze in makeMaze.
*/
SquareMaze::SquareMaze() {
  struct timeval time;
	gettimeofday(&time, NULL);
	srand((time.tv_sec * 1000) + time.tv_usec);
}

/*
* This function initializes the width and height of the maze, the boolean arrays and the disjoint set.
* It then randomly generates the maze such that there is exactly one path to each cell (no cycles) by removing
* walls until the disjoint set object has every cell in one set. It also ensures that the right walls
* of the rightmost column and the down walls of the bottom row cannot be removed.
*/
void SquareMaze::makeMaze(int width, int height) {
  width_ = width;
  height_ = height;
  xend_ = 0;

  down_.resize(width_);
  right_.resize(width_);
  visited_.resize(width_);
  for(int i = 0; i < width_; i++) {
    down_[i].resize(height_);
    right_[i].resize(height_);
    visited_[i].resize(height_);
    for(int j = 0; j < height_; j++) {
      down_[i][j] = true;
      right_[i][j] = true;
      visited_[i][j] = false;
    }
  }

  if(!endpaths_.empty())
    endpaths_.clear();

  cells_.clear();
  cells_.addelements(width_ * height_);

  while(!mazeDone()) {
    int x = rand() % width_;
    int y = rand() % height_;
    int dir = rand() % 2;
    if(((x == width_ - 1) && dir == 0) || ((y == height_ - 1) && dir == 1))
      continue;
    if(dir == 0) {
      if(cells_.find(x + (y * width_)) != cells_.find((x + 1) + (y * width_))) {
        right_[x][y] = false;
        cells_.setunion(x + (y * width_), (x + 1) + (y * width_));
      }
    }
    else {
      if(cells_.find(x + (y * width_)) != cells_.find(x + ((y + 1) * width_))) {
        down_[x][y] = false;
        cells_.setunion(x + (y * width_), x + ((y + 1) * width_));
      }
    }
  }
}

/*
* This function decides whether it is possible to travel in a certain direction from a cell
* without going through any walls. It decides this by looking up the cell in question by its coordinates
* and the provided direction.
* If dir = 0, want to travel right (look at right_[x][y] to see if wall exists there).
* If dir = 1, want to travel down (look at down_[x][y] to see if wall exists there).
* If dir = 2, want to travel left (look at right_[x-1][y] to see if right wall exists for cell to the left).
* If dir = 3, want to travel down (look at down_[x][y-1] to see if down wall exists for cell to the top).
*/
bool SquareMaze::canTravel(int x, int y, int dir) const {
  if(dir == 0) {  // x + 1 (right)
    if((x + 1 == width_) || right_[x][y])
      return false;
    else
      return true;
  }
  else if(dir == 1) {  // y + 1 (down)
    if((y + 1 == height_) || down_[x][y])
      return false;
    else
      return true;
  }
  else if(dir == 2) {  // x - 1 (left)
    if((x == 0) || right_[x-1][y])
      return false;
    else
      return true;
  }
  else if(dir == 3) {  // y - 1 (up)
    if((y == 0) || down_[x][y-1])
      return false;
    else
      return true;
  }
  return false;
}

/*
* This function creates a wall by changing the down_ and right_ boolean arrays.
* It is given the x and y coordinates of the cell, which wall to change (dir = 0: right wall, dir = 1: down wall),
* and whether to create a wall or remove the existing wall at the given location.
*/
void SquareMaze::setWall(int x, int y, int dir, bool exists) {
  if(dir == 0)
    right_[x][y] = exists;
  if(dir == 1)
    down_[x][y] = exists;
}

/*
* This function solves the maze by calling the DFS traversal function mazeDFS, which populates
* a hash table storing all possible solutions to the maze (paths ending at a cell in the bottom row).
* The function iterates through the hash table, finding the longest solution path (ties broken in favor of
* a smaller x coordinate of the ending cell). The solution vector contains integers representing the
* sequence of steps needed to complete the maze (0: right, 1: down, 2: left, 3: up).
*/
vector<int> SquareMaze::solveMaze() {
  unsigned max_size = 0;
  int x_end = 0;
  vector<int> directions;
  vector<int> solution;

  mazeDFS(0, 0, directions);

  for(unordered_map<int, vector<int>>::iterator it = endpaths_.begin(); it != endpaths_.end(); ++it) {
    if(it->second.size() > max_size) {
      solution = it->second;
      max_size = it->second.size();
      x_end = it->first;
    }
    else if(it->second.size() == max_size) {
      if(it->first < x_end) {
        solution = it->second;
        x_end = it->first;
      }
    }
  }

  xend_ = x_end;
  return solution;
}

/*
* This function draws the randomly generated maze without its solution. Each wall is drawn
* as a line of black pixels. It leaves the top wall of the top left cell open to represent the
* beginning of the maze.
*/
PNG* SquareMaze::drawMaze() const {
  PNG* maze = new PNG((width_ * 10) + 1, (height_ * 10) + 1);
  for(int x = 0; x < (width_ * 10) + 1; x++) {
    if(x < 1 || x > 9) {
      HSLAPixel& mypixel = maze->getPixel(x, 0);
      mypixel.l = 0;
    }
  }
  for(int y = 1; y < (height_ * 10) + 1; y++) {
      HSLAPixel& mypixel = maze->getPixel(0, y);
      mypixel.l = 0;
  }

  for(int x = 0; x < width_; x++) {
    for(int y = 0; y < height_; y++) {
      if(right_[x][y]) {
        for(int k = 0; k <= 10; k++) {
            HSLAPixel& mypixel = maze->getPixel((x + 1) * 10, (y * 10) + k);
            mypixel.l = 0;
        }
      }
      if(down_[x][y]) {
        for(int k = 0; k <= 10; k++) {
            HSLAPixel& mypixel = maze->getPixel((x * 10) + k, (y + 1) * 10);
            mypixel.l = 0;
        }
      }
    }
  }

  return maze;
}

/*
* This function uses the PNG created by drawMaze and draws a red path through the maze corresponding
* to the solution computed by solveMaze. It also removes the bottom wall of the ending cell in the PNG.
*/
PNG* SquareMaze::drawMazeWithSolution() {
  PNG* final_maze = drawMaze();
  vector<int> solution = solveMaze();
  HSLAPixel red(0, 1, 0.5, 1);
  int x = 5;
  int y = 5;

  for(unsigned i = 0; i < solution.size(); i++) {
    if(solution[i] == 0) {  // x + 1 (right)
      for(int k = 0; k <= 10; k++) {
          HSLAPixel& mypixel = final_maze->getPixel(x + k, y);
          mypixel = red;
      }
      x += 10;
    }
    else if(solution[i] == 1) {  // y + 1 (down)
      for(int k = 0; k <= 10; k++) {
          HSLAPixel& mypixel = final_maze->getPixel(x, y + k);
          mypixel = red;
      }
      y += 10;
    }
    else if(solution[i] == 2) {  // x - 1 (left)
      for(int k = 0; k <= 10; k++) {
          HSLAPixel& mypixel = final_maze->getPixel(x - k, y);
          mypixel = red;
      }
      x -= 10;
    }
    else if(solution[i] == 3) {  // y - 1 (up)
      for(int k = 0; k <= 10; k++) {
          HSLAPixel& mypixel = final_maze->getPixel(x, y - k);
          mypixel = red;
      }
      y -= 10;
    }
  }

  for(int k = 1; k < 10; k++) {
      HSLAPixel& mypixel = final_maze->getPixel((xend_ * 10) + k, height_ * 10);
      mypixel.l = 1;
  }

  return final_maze;
}

/*
* This function draws a creative maze and its solution using an input PNG as a background.
*/
PNG* SquareMaze::drawCreativeMaze(PNG& background) {
  PNG* final_maze = &background;
  final_maze->resize((width_ * 10) + 1, (height_ * 10) + 1);

  HSLAPixel orange(11, 1, 0.5, 1);
  HSLAPixel blue(216, 1, 0.4, 1);
  HSLAPixel purple(299,100,50);

  int color_gen;
  for(int x = 0; x < (width_ * 10) + 1; x++) {
    HSLAPixel& mypixel = final_maze->getPixel(x, 0);
    if(x % 10 == 0)
      color_gen = rand() % 3;
    if(x < 1 || x > 9) {
      if(color_gen == 1)
        mypixel = orange;
      else if(color_gen == 2)
        mypixel = blue;
      else
        mypixel = purple;
    }
    else
      mypixel.l = 1;
  }
  for(int y = 1; y < (height_ * 10) + 1; y++) {
      HSLAPixel& mypixel = final_maze->getPixel(0, y);
      if(y % 10 == 0)
        color_gen = rand() % 2;
        if(color_gen == 1)
          mypixel = orange;
        else if(color_gen == 2)
          mypixel = blue;
        else
          mypixel = purple;
  }

  for(int x = 0; x < width_; x++) {
    for(int y = 0; y < height_; y++) {
      if(right_[x][y]) {
        int random = rand() % 2;
        for(int k = 0; k <= 10; k++) {
            HSLAPixel& mypixel = final_maze->getPixel((x + 1) * 10, (y * 10) + k);
            if(color_gen == 1)
              mypixel = orange;
            else if(color_gen == 2)
              mypixel = blue;
            else
              mypixel = purple;
        }
      }
      if(down_[x][y]) {
        int random = rand() % 2;
        for(int k = 0; k <= 10; k++) {
            HSLAPixel& mypixel = final_maze->getPixel((x * 10) + k, (y + 1) * 10);
            if(color_gen == 1)
              mypixel = orange;
            else if(color_gen == 2)
              mypixel = blue;
            else
              mypixel = purple;
        }
      }
    }
  }

  vector<int> solution = solveMaze();
  int x = 5;
  int y = 5;

  for(unsigned i = 0; i < solution.size(); i++) {
    if(solution[i] == 0) {  // x + 1 (right)
      for(int k = 0; k <= 10; k++) {
          HSLAPixel& mypixel = final_maze->getPixel(x + k, y);
          mypixel.l = 0;
      }
      x += 10;
    }
    else if(solution[i] == 1) {  // y + 1 (down)
      for(int k = 0; k <= 10; k++) {
          HSLAPixel& mypixel = final_maze->getPixel(x, y + k);
          mypixel.l = 0;
      }
      y += 10;
    }
    else if(solution[i] == 2) {  // x - 1 (left)
      for(int k = 0; k <= 10; k++) {
          HSLAPixel& mypixel = final_maze->getPixel(x - k, y);
          mypixel.l = 0;
      }
      x -= 10;
    }
    else if(solution[i] == 3) {  // y - 1 (up)
      for(int k = 0; k <= 10; k++) {
          HSLAPixel& mypixel = final_maze->getPixel(x, y - k);
          mypixel.l = 0;
      }
      y -= 10;
    }
  }

  for(int k = 1; k < 10; k++) {
      HSLAPixel& mypixel = final_maze->getPixel((xend_ * 10) + k, height_ * 10);
      mypixel.l = 1;
  }

  return final_maze;
}

/*
* This helper function uses the DisjointSets class to find the size of the up-tree containing the first cell.
* Once this size is equal to the number of cells in the maze, maze generation is complete.
*/
bool SquareMaze::mazeDone() {
  return cells_.size(0) == (width_ * height_);
}

/*
* This helper function performs a DFS traversal of the cells in the maze, maintaining a vector
* that stores directions to each cell. This traversal checks to make sure that it doesn't visit
* any cell more than once and that it doesn't go through any walls.
* If the input cell to the function is on the bottom row,
* the directions vector is copied into a hash table. Once the hash table has been filled
* with a number of paths equal to the width of the maze, the traversal stops.
*/
void SquareMaze::mazeDFS(int x, int y, vector<int>& directions) {
  if(endpaths_.size() == (unsigned)width_)
    return;
  visited_[x][y] = true;
  if(y == height_ - 1)
    endpaths_[x] = directions;
  for(int i = 0; i < 4; i++) {
    if(canTravel(x, y, i) && !beenVisited(x, y, i)) {
      directions.push_back(i);
      if(i == 0)  // x + 1 (right)
        mazeDFS(x + 1, y, directions);
      else if(i == 1)  // y + 1 (down)
        mazeDFS(x, y + 1, directions);
      else if(i == 2)  // x - 1 (left)
        mazeDFS(x - 1, y, directions);
      else if(i == 3)  // y - 1 (up)
        mazeDFS(x, y - 1, directions);
    }
  }
  directions.pop_back();
}

/*
* This helper function accepts an input cell's coordinates and the desired travel direction from the cell
* and determines whether the adjacent cell in the corresponding direction has been visited already.
* This is computed by looking up information from the "vistsed" array.
*/
bool SquareMaze::beenVisited(int x, int y, int dir) {
  if(dir == 0)  // x + 1 (right)
    return visited_[x + 1][y];
  else if(dir == 1)  // y + 1 (down)
    return visited_[x][y + 1];
  else if(dir == 2)  // x - 1 (left)
    return visited_[x - 1][y];
  else if(dir == 3)  // y - 1 (up)
    return visited_[x][y - 1];
  return true;
}
