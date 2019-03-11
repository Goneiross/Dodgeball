#include "map.h"
#ifndef TOOLS_H
#define TOOLS_H
#include "tools.h"
#endif

using namespace std;

Obstacle::Obstacle(double x, double y, double s){
    xPosition = x;
    yPosition = y;
    hitbox = new Square(xPosition, yPosition, s);
}
double Obstacle::getX () const {return xPosition;}
double Obstacle::getY () const {return yPosition;}
Square* Obstacle::getHitbox() const {return hitbox;}

Map::Map(int xSize, int ySize){
    grid = new bool*[10];
    for (int i = 0; i < 10; i++) {
        grid[i] = new bool[10];
        for (int j = 0; j < 10; j++) {
            grid[i][j] = false;
        }
    }
}
double Map::getX () const {return xSize;}
double Map::getY () const {return ySize;}
void Map::addObstacle(int xPosition, int yPosition){
    grid[xPosition][yPosition] = true;
}
void Map::removeObstacle(int xPosition, int yPosition){
    grid[xPosition][yPosition] = false;
}
bool Map::isObstacle(double x, double y) const{return grid[int(x)][int(y)];}