/*!
  \file   ball.h
  \author Guillaume Pellerin & Vincent Miche
  \date   March 2019
  \brief  "Ball" module header
*/

#ifndef TOOLS_H
#define TOOLS_H
#include "tools.h"
#endif

class Ball {
public:
  Ball(double x, double y, double angle, int nbCell);
  ~Ball(){};
  double getX() const;
  double getY() const;
  double getRadius() const;
  Circle *getHitbox() const;
  void updatePosition();

private:
  double velocity;
  double angle;
  Circle *hitbox;
};