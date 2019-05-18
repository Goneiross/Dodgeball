/*!
  \file   ball.h
  \author Guillaume Pellerin & Vincent Miche
  \date   March 2019
  \brief  "Ball" module header
*/

#ifndef BALL_H
#define BALL_H

#include <vector>

#include "map.h"
#include "tools.h"

class Ball {
public:
  Ball(double xPosition, double yPosition, double angle, double radius,
       double velocity, int lPosition, int cPosition, int ID);
  ~Ball(){};
  double getX() const;
  double getY() const;
  double getL() const;
  double getC() const;
  double getGX() const;
  double getGY() const;
  double getRadius() const;
  double getAngle() const;
  double getVelocity() const;
  int getID() const;
  Circle *getHitbox() const;
  void setGX(double gY);
  void setGY(double gY);
  void setL(int l);
  void setC(int c);

private:
  int ID;
  int lgnPos, colPos;
  double gXPosition, gYPosition;
  double velocity;
  double angle;
  Circle *hitbox;
};

class BallMap : public Map {
public:
  BallMap(int l, int c);
  void addBall(double xPosition, double yPosition, double angle, double radius,
               double velocity, int ID);
  void removeBall(int ID);
  void removeAll();
  void reserveSpace(int nbBall);
  void updatePosition();
  bool isBall(int lgnPos, int colPos);
  std::vector<int> whichBall(int lgnPos, int colPos);
  Ball *getBall(int index) const;
  int getNb() const;
  int getNewID() const;

private:
  std::vector<Ball *> balls;
  std::vector<int> **ballGrid;
};

#endif
