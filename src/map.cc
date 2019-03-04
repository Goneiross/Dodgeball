#include "map.h"
#include "tools.h"

using namespace std;

Map::Map(int xSize, int ySize){
    bool** grid = new bool*[xSize];
        for (int i = 0; i < xSize; i++) {
            grid[i] = new bool[ySize];
            for (int j = 0; j < ySize; j++) {
                grid[i][j] = false;
            }
        }
}
void Map::addObstacle(int xPosition, int yPosition){
    grid[xPosition][yPosition] = true;
}
void Map::removeObstacle(int xPosition, int yPosition){
    grid[xPosition][yPosition] = false;
}