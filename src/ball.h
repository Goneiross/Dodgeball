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
  double getGX() const;
  double getGY() const;
  double setGX(double gY);
  double setGY(double gY);
  double getRadius() const;
  Circle *getHitbox() const;
  double getAngle() const;
  void updatePosition();

private:
  int ID;
  double velocity;
  double angle;
  Circle *hitbox;
  int lPosition, cPosition;
  double gXPosition, gYPosition;
};

class BallMap {
  public:
    BallMap(int l, int c);
    void addBall(double xPosition, double yPosition, double angle, 
                double radius, double velocity, int ID);
    void removeBall(int ID);
    void removeAll();
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