#include "map.h"
#include "tools.h"

using namespace std;

Map::Map(int xSize, int ySize){
    grid = new bool*[10];
    for (int i = 0; i < 10; i++) {
        grid[i] = new bool[10];
        for (int j = 0; j < 10; j++) {
            grid[i][j] = false;
        }
    }
}
double Ball::getX () const {return xSize;}
double Ball::getY () const {return ySize;}
void Map::addObstacle(int xPosition, int yPosition){
    grid[xPosition][yPosition] = true;
}
void Map::removeObstacle(int xPosition, int yPosition){
    grid[xPosition][yPosition] = false;
}
bool Map::isObstacle(double x, double y) const{return grid[int(x)][int(y)];}