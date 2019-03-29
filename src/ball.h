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
  Ball(double xPosition, double yPosition, double angle, double radius, 
      double velocity, int lPosition, int cPosition, int ID);
  ~Ball(){};
  double getX() const;
  double getY() const;
  double getL() const;
  double getC() const;
  double getRadius() const;
  Circle *getHitbox() const;
  void updatePosition();

private:
  int ID;
  double velocity;
  double angle;
  Circle *hitbox;
  int lPosition;
  int cPosition;
};

class BallMap {
  public:
    BallMap(int l, int c);
    void addBall(double xPosition, double yPosition, double angle, 
                double radius, double velocity, int ID);
    void reserveSpace(int nbBall);
    bool isBall(int lPosition, int cPosition);
    std::vector<int> whichBall(int lPosition, int cPosition);
    Ball* getBall(int index) const;
    int getNb () const;
  private:
    int lineNumber;
    int columnNumber;
    std::vector<Ball *> balls;
    std::vector<int>** ballGrid;
};