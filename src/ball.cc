/*!
  \file   ball.cc
  \author Guillaume Pellerin & Vincent Miche
  \date   March 2019
  \brief  "Ball" module implementation
*/
#include <vector>
#include <math.h>
#include "define.h"
#include "ball.h"
#ifndef TOOLS_H
#define TOOLS_H
#include "tools.h"
#endif

using namespace std;

Ball::Ball(double x0, double y0, double a, int nbCell, int i) {
  angle = a;
  ID = i;
  double radius = COEF_RAYON_BALLE * (SIDE / nbCell);
  velocity = COEF_VITESSE_BALLE * (SIDE / nbCell);
  hitbox = new Circle(x0, y0, radius);
}
Ball::Ball(double x0, double y0, double a, double r, double v, int i) {
  angle = a;
  ID = i;
  velocity = v;
  hitbox = new Circle(x0, y0, r);
}
void Ball::updatePosition() {
  hitbox->setX(hitbox->getX() + cos(angle) * velocity);
  hitbox->setY(hitbox->getY() + sin(angle) * velocity);
}
double Ball::getX() const { return hitbox->getX(); }
double Ball::getY() const { return hitbox->getY(); }
double Ball::getRadius() const { return hitbox->getRadius(); }
Circle *Ball::getHitbox() const { return hitbox; }

BallMap::BallMap(int l, int c){
  lineNumber = l;
  columnNumber = c;

  ballGrid = new vector<int>* [lineNumber];
  for (int i = 0; i < lineNumber; i++) {
    ballGrid[i] = new vector<int>[columnNumber];
    for (int j = 0; j < columnNumber; j++) {
      ballGrid[i][j].push_back(-1);
    }
  }
}

void BallMap::addBall(double x, double y, double angle, int nbCell, int ID){
  balls.push_back(new Ball(x, y, angle, nbCell, ID));
  int cPosition = ((x + DIM_MAX ) / columnNumber ) - 1 / 2;
  int lPosition = - ((y - DIM_MAX) / lineNumber ) - 1 / 2;
  if (ballGrid[lPosition][cPosition][0] == -1){
    ballGrid[lPosition][cPosition][0] = ID;
  } else {
    ballGrid[lPosition][cPosition].push_back(ID);
  }
}