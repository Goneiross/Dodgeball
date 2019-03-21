/*!
  \file   map.h
  \author Guillaume Pellerin & Vincent Miche
  \date   March 2019
  \brief  "Map" module header
*/

#include <vector>

#ifndef TOOLS_H
#define TOOLS_H
#include "tools.h"
#endif

class Obstacle {
public:
  Obstacle(double x0, double y0, double s);
  ~Obstacle(){};
  double getX() const;
  double getY() const;
  Square *getHitbox() const;

private:
  Square *hitbox;
};

class Map {
public:
  Map(int l, int c);
  ~Map(){};
  double getX() const;
  double getY() const;
  std::vector<Obstacle *> getObstacle() const;
  void addObstacle(int lPosition, int cPosition, int ID);
  void removeObstacle(int lPosition, int cPosition);
  bool isObstacle(double lPosition, double cPosition);
  int whichObstacle(double lPosition, double cPosition);

private:
  int **obstaclesGrid;
  int lineNumber;
  int columnNumber;
  std::vector<Obstacle *> obstacles;
};