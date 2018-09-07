#include <iostream>
#include "dsets.h"
#include "maze.h"
#include "cs225/PNG.h"

using namespace std;

int main()
{
     PNG creativeMaze;
    creativeMaze.readFromFile("background.png");

     SquareMaze maze;
     maze.makeMaze(119,79);
     PNG* final_maze = maze.drawCreativeMaze(creativeMaze);

    final_maze->writeToFile("creative.png");

    return 0;
}
