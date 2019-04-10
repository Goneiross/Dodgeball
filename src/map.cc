/*!
  \file   map.cc
  \author Guillaume Pellerin & Vincent Miche
  \date   March 2019
  \brief  "Map" module implementation
*/

#include "map.h"
#include "define.h"
#include <iostream>
#include <vector>
#ifndef TOOLS_H
#define TOOLS_H
#include "tools.h"
#endif

using namespace std;

Obstacle::Obstacle(double x0, double y0, int l, int c, double s) {
    hitbox = new Square(x0, y0, s);
    cPosition = c;
    lPosition = l;
    gXPosition = -1;
    gYPosition = -1;
}
int Obstacle::getL() const { return lPosition; }
int Obstacle::getC() const { return cPosition; }
double Obstacle::getX() const { return hitbox->getX(); }
double Obstacle::getY() const { return hitbox->getY(); }
double Obstacle::getGX() const { return gXPosition; }
double Obstacle::getGY() const { return gYPosition; }
double Obstacle::getSide() const { return hitbox->getSide(); }
void Obstacle::setGX(double gX) { gXPosition = gX; }
void Obstacle::setGY(double gY) { gYPosition = gY; }
Square* Obstacle::getHitbox() const { return hitbox; }

Map::Map(int l, int c) {
    lineNumber = l;
    columnNumber = c;
    obstaclesGrid = new int*[lineNumber];
    for (int i = 0; i < lineNumber; i++) {
        obstaclesGrid[i] = new int[columnNumber];
        for (int j = 0; j < columnNumber; j++) {
            obstaclesGrid[i][j] = -1;
        }
    }
}
int Map::getLNb() const { return lineNumber; }
int Map::getCNb() const { return columnNumber; }
int Map::getNb() const { return obstacles.size(); }
Obstacle* Map::getObstacle(int i) const { return obstacles[i]; }

void Map::addObstacle(int lPosition, int cPosition, int ID) {
    obstaclesGrid[lPosition][cPosition] = ID;
    double S = SIDE / lineNumber;
    double X = (0.5 * S + cPosition * S) - (DIM_MAX);
    double Y = -(0.5 * S + lPosition * S) + (DIM_MAX);
    obstacles.push_back(new Obstacle(X, Y, lPosition, cPosition, S));
}
void Map::removeObstacle(int ID) {
    int l = obstacles[ID]->getL();
    int c = obstacles[ID]->getC();
    obstacles.erase(obstacles.begin() + ID);
    obstaclesGrid[l][c] = -1;
}
void Map::removeAll() {
    int obstacleNb = obstacles.size();
    for (int i = 0; i < obstacleNb; i++) {
        removeObstacle(0);
    }
}
bool Map::isObstacle(double lPosition, double cPosition) {
    if (obstaclesGrid[int(lPosition)][int(cPosition)] == -1) {
        return false;
    } else {
        return true;
    }
}
int Map::whichObstacle(double lPosition, double cPosition) {
    return obstaclesGrid[int(lPosition)][int(cPosition)];
}