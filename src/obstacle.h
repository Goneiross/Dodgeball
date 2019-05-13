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
  	int getL() const;
  	int getC() const;
  	double getGX() const;
  	double getGY() const;
  	void setGX(double x);
  	void setGY(double y);
  	double getSide() const;
  	Square* getHitbox() const;

private:
  	Square* hitbox;
  	int colPos;
  	int lgnPos;
  	double gXPosition;
  	double gYPosition;
};

class ObstacleMap: public Map {
public:
  	ObstacleMap(int l, int c);
  	~ObstacleMap(){};
  	int getNb() const;
  	Obstacle* getObstacle(int i) const;
  	void addObstacle(int lgnPos, int colPos, int ID);
  	void removeObstacle(int ID);
  	void removeAll();
  	bool isObstacle(double lgnPos, double colPos);
  	int whichObstacle(double lgnPos, double colPos);

private:
  	int** obstaclesGrid;
  	std::vector<Obstacle *> obstacles;
};

#endif