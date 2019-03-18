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
  Map(int lPosition, int cPosition);
  ~Map(){};
  double getX() const;
  double getY() const;
  std::vector<Obstacle *> getObstacle() const;
  void addObstacle(int xPosition, int yPosition);
  void removeObstacle(int xPosition, int yPosition);
  bool isObstacle(double x, double y) const;
  void test();

private:
  bool **grid;
  int xSize;
  int ySize;
  std::vector<Obstacle *> obstacles;
};