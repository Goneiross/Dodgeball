#include <vector>
#include "define.h"
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

Map::Map(int x, int y){
    xSize = x;
    ySize = y;
    grid = new bool*[xSize];
    for (int i = 0; i < xSize; i++) {
        grid[i] = new bool[ySize];
        for (int j = 0; j < ySize; j++) {
            grid[i][j] = false;
        }
    }
}
double Map::getX () const {return xSize;}
double Map::getY () const {return ySize;}
vector<Obstacle*> Map::getObstacle () const {return obstacles;}
void Map::addObstacle(int xPosition, int yPosition){
    grid[xPosition][yPosition] = true;
    double S = SIDE / xSize ;
    double X = 1/2 * S + xPosition * S;
    double Y = 1/2 * S + yPosition * S;
    obstacles.push_back(new Obstacle(X, Y, S));
}
void Map::removeObstacle(int xPosition, int yPosition){
    grid[xPosition][yPosition] = false;
}
bool Map::isObstacle(double x, double y) const{return grid[int(x)][int(y)];}