#include <vector>
#include <iostream>
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
void Map::addObstacle(int lPosition, int cPosition){
    grid[lPosition][cPosition] = true;
    double S = SIDE / xSize ;
    double X = (0.5 * S + cPosition * S) - (DIM_MAX);
    double Y = -(0.5 * S + lPosition * S) + (DIM_MAX);
    obstacles.push_back(new Obstacle(X, Y, S));
}
void Map::removeObstacle(int lPosition, int cPosition){
    grid[lPosition][cPosition] = false;
}
bool Map::isObstacle(double x, double y) const{return grid[int(x)][int(y)];}