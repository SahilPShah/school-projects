#ifndef MAZE_H_
#define MAZE_H_

#include "cs225/PNG.h"
#include "dsets.h"
#include <vector>
#include <unordered_map>

using std::vector;
using std::unordered_map;
using cs225::PNG;
using cs225::HSLAPixel;

class SquareMaze {
public:
  /*
  * The constructor of a SquareMaze simply initializes the random number seed that will be used
  * to randomly generate the maze in makeMaze.
  */
  SquareMaze();

  /*
  * This function initializes the width and height of the maze, the boolean arrays and the disjoint set.
  * It then randomly generates the maze such that there is exactly one path to each cell (no cycles) by removing
  * walls until the disjoint set object has every cell in one set. It also ensures that the right walls
  * of the rightmost column and the down walls of the bottom row cannot be removed.
  */
  void makeMaze(int width, int height);

  /*
  * This function decides whether it is possible to travel in a certain direction from a cell
  * without going through any walls. It decides this by looking up the cell in question by its coordinates
  * and the provided direction.
  * If dir = 0, want to travel right (look at right_[x][y] to see if wall exists there).
  * If dir = 1, want to travel down (look at down_[x][y] to see if wall exists there).
  * If dir = 2, want to travel left (look at right_[x-1][y] to see if right wall exists for cell to the left).
  * If dir = 3, want to travel down (look at down_[x][y-1] to see if down wall exists for cell to the top).
  */
  bool canTravel(int x, int y, int dir) const;

  /*
  * This function creates a wall by changing the down_ and right_ boolean arrays.
  * It is given the x and y coordinates of the cell, which wall to change (dir = 0: right wall, dir = 1: down wall),
  * and whether to create a wall or remove the existing wall at the given location.
  */
  void setWall(int x, int y, int dir, bool exists);

  /*
  * This function solves the maze by calling the DFS traversal function mazeDFS, which populates
  * a hash table storing all possible solutions to the maze (paths ending at a cell in the bottom row).
  * The function iterates through the hash table, finding the longest solution path (ties broken in favor of
  * a smaller x coordinate of the ending cell). The solution vector contains integers representing the
  * sequence of steps needed to complete the maze (0: right, 1: down, 2: left, 3: up).
  */
  vector<int> solveMaze();

  /*
  * This function draws the randomly generated maze without its solution. Each wall is drawn
  * as a line of black pixels. It leaves the top wall of the top left cell open to represent the
  * beginning of the maze.
  */
  PNG* drawMaze() const;

  /*
  * This function uses the PNG created by drawMaze and draws a red path through the maze corresponding
  * to the solution computed by solveMaze. It also removes the bottom wall of the ending cell in the PNG.
  */
  PNG* drawMazeWithSolution();

  /*
  * This function draws a creative maze and its solution using an input PNG as a background.
  */
  PNG* drawCreativeMaze(PNG& background);

private:
  /*
  * This helper function uses the DisjointSets class to find the size of the up-tree containing the first cell.
  * Once this size is equal to the number of cells in the maze, maze generation is complete.
  */
  bool mazeDone();

  /*
  * This helper function performs a DFS traversal of the cells in the maze, maintaining a vector
  * that stores directions to each cell. This traversal checks to make sure that it doesn't visit
  * any cell more than once and that it doesn't go through any walls.
  * If the input cell to the function is on the bottom row,
  * the directions vector is copied into a hash table. Once the hash table has been filled
  * with a number of paths equal to the width of the maze, the traversal stops.
  */
  void mazeDFS(int x, int y, vector<int>& directions);

  /*
  * This helper function accepts an input cell's coordinates and the desired travel direction from the cell
  * and determines whether the adjacent cell in the corresponding direction has been visited already.
  * This is computed by looking up information from the "vistsed" array.
  */
  bool beenVisited(int x, int y, int dir);

  int width_;     // variable storing the width of the square maze
  int height_;    // variable storing the height of the square maze
  int xend_;      // variable storing the x coordinate of the ending cell (calculated in solveMaze)

  vector<vector<bool>> down_;     // boolean array for the bottom wall of each cell (true: wall exists, false: wall doesn't exist)
  vector<vector<bool>> right_;    // boolean array for the right wall of each cell (true: wall exists, false: wall doesn't exist)
  vector<vector<bool>> visited_;  // boolean array keeping tack of which cells have been visited when doing DFS traversal

  unordered_map<int, vector<int>> endpaths_;  // hash table storing path directions to each cell in bottom row of maze
                                              // indexed by their ending x coordinates
  DisjointSets cells_;              // disjoint set used to make sure there is exactly one path to each cell in maze
};

#endif
