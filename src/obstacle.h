/*!
  \file   obstacle.h
  \author Guillaume Pellerin & Vincent Miche
  \date   May 2019
  \brief  "Obstacle" module header
*/

#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <vector>

#include "map.h"
#include "tools.h"

class Obstacle {
public:
  Obstacle(double x0, double y0, int lgnPos, int colPos, double s);
  ~Obstacle(){};
  double getX() const;
  double getY() const;
  double getGX() const;
  double getGY() const;
  int getL() const;
  int getC() const;
  double getSide() const;
  Square *getHitbox() const;
  void setGX(double x);
  void setGY(double y);

private:
  Square *hitbox;
  int colPos, lgnPos;
  double gXPosition, gYPosition;
};

class ObstacleMap : public Map {
public:
  ObstacleMap(int l, int c);
  ~ObstacleMap(){};
  void addObstacle(int lgnPos, int colPos, int ID);
  void removeObstacle(int ID);
  void removeAll();
  bool isObstacle(double lgnPos, double colPos);
  int whichObstacle(double lgnPos, double colPos);
  int getNb() const;
  Obstacle *getObstacle(int i) const;

private:
  int **obstaclesGrid;
  std::vector<Obstacle *> obstacles;
};

#endif