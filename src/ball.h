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
  Ball(double x, double y, double angle, int nbCell, int i);
  Ball(double x, double y, double angle, double r, double v, int i);
  ~Ball(){};
  double getX() const;
  double getY() const;
  double getRadius() const;
  Circle *getHitbox() const;
  void updatePosition();

private:
  int ID;
  double velocity;
  double angle;
  Circle *hitbox;
};

class BallMap {
  public:
    BallMap(int l, int c);
    void addBall(double x, double y, double angle, double r, double v, int ID);
    void reserveSpace(int nbBall);
    Ball* getBall(int i) const;
  private:
    int lineNumber;
    int columnNumber;
    std::vector<Ball*> balls;
    std::vector<int>** ballGrid;
};