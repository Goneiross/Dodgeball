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
  Obstacle(double x0, double y0, int lPosition, int cPosition, double s);
  ~Obstacle(){};
  double getX() const;
  double getY() const;
  double getL() const;
  double getC() const;
  Square* getHitbox() const;

private:
  Square* hitbox;
  int cPosition;
  int lPosition;
};

class Map {
public:
  Map(int l, int c);
  ~Map(){};
  int getLNb() const;
  int getCNb() const;
  int getNb() const;
  Obstacle* getObstacle(int i) const;
  void addObstacle(int lPosition, int cPosition, int ID);
  void removeObstacle(int ID);
  bool isObstacle(double lPosition, double cPosition);
  int whichObstacle(double lPosition, double cPosition);

private:
  int** obstaclesGrid;
  int lineNumber;
  int columnNumber;
  std::vector<Obstacle *> obstacles;
};