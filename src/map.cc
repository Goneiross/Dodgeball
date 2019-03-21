/*!
  \file   map.cc
  \author Guillaume Pellerin & Vincent Miche
  \date   March 2019
  \brief  "Map" module implementation
*/

#include <vector>
#include <iostream>
#include "define.h"
#include "map.h"
#ifndef TOOLS_H
#define TOOLS_H
#include "tools.h"
#endif

using namespace std;

Obstacle::Obstacle(double x0, double y0, double s) {
  hitbox = new Square(x0, y0, s);
}
double Obstacle::getX() const { return hitbox->getX(); }
double Obstacle::getY() const { return hitbox->getY(); }
Square *Obstacle::getHitbox() const { return hitbox; }

Map::Map(int x, int y) {
  lineNumber = x;
  columnNumber = y;
  obstaclesGrid = new int *[lineNumber];
  for (int i = 0; i < lineNumber; i++) {
    obstaclesGrid[i] = new int[columnNumber];
    for (int j = 0; j < columnNumber; j++) {
      obstaclesGrid[i][j] = -1;
    }
  }
}
double Map::getX() const { return lineNumber; }
double Map::getY() const { return columnNumber; }
vector<Obstacle *> Map::getObstacle() const { return obstacles; }

void Map::addObstacle(int lPosition, int cPosition, int ID) {
  obstaclesGrid[lPosition][cPosition] = ID;
  double S = SIDE / lineNumber;
  double X = (0.5 * S + cPosition * S) - (DIM_MAX);
  double Y = -(0.5 * S + lPosition * S) + (DIM_MAX);
  obstacles.push_back(new Obstacle(X, Y, S));
}
void Map::removeObstacle(int lPosition, int cPosition) {
  obstaclesGrid[lPosition][cPosition] = -1;
}
bool Map::isObstacle(double lPosition, double cPosition) { 
  if(obstaclesGrid[int(lPosition)][int(cPosition)] == -1){
    return false; 
  } else {
    return true;
  }
}
int Map::whichObstacle(double lPosition, double cPosition) {
  return obstaclesGrid[int(lPosition)][int(cPosition)]; 
}